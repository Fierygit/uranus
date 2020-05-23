//
// Created by Firefly on 2020/5/18.
//

#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "KeepAlive.h"
#include "../common/Util.h"
#include "../common/loguru.hpp"
#include "../common/WaitGroup.h"
#include "syncDataHandler.h"


//使用引用， 并发的时候遍历会不会出问题？//
// 不会， 因为大小不会变化， 里面使用分段锁
void KeepAlive::init(Participants &participants, uranus::ThreadPool *poll) {
    for (Participant *p : participants) if (p->isAlive) p->lastAlive = std::chrono::system_clock::now();
    std::thread tmp{[&] {
        while (!Finished) {

            std::this_thread::sleep_for(std::chrono::seconds(this->checkInterval));
            std::atomic<bool> needSyncData{false};
            keepaliveCheck(participants, needSyncData, poll);
            if(needSyncData) syncKVDB(participants);
            //todo 确保了同步时不会有新的上线， 会不会有冲突呢？ 有！！！
            // 同步过程中， client 各个pa返回的值可能不一样！！！
        }
    }};
    tmp.detach();
    LOG_F(INFO, "start to keepalive check!!");
}

// 检测心跳在这里
// 检测到没有反应尝试连接后直接挂了p
// 1、如果是网络问题
// 2、 p故障
//      如果心跳没返回来，都重新换一条连接
//


void KeepAlive::keepaliveCheck(Participants &participants, std::atomic<bool> &needSyncData, uranus::ThreadPool *pool) {
    WaitGroup waitGroup;
    waitGroup.Add("keepAlive", participants.size());
    //std::cout << "what the fuck\n" << participants.size() << std::endl;
    for (Participant *p : participants) {
        pool->addTask([p, &needSyncData, &pool, this, &waitGroup] { // p不能是 引用!!!!!!!!!!!!!!!!检查所有的使用
            Time now = std::chrono::system_clock::now();
            Munites diff = std::chrono::duration_cast<Munites>(now - p->lastAlive);
            {       //RAII
                //std::cout << "what the fuck\n";
                std::unique_lock<std::mutex> tmpLock(p->lock); //warning
                //std::cout << "what the fuck\n";
                if (!p->isAlive) {// 挂了的尝试去连接一下
                    if (connectLostPa(p)) needSyncData = true;
                    goto end;  // 结束喽！！！！！！！！！！！！！！！！！！！！！！
                }
                if (diff.count() > invalidInterval.count()) {  // 如果长时间没有回来， 挂掉！！！
                    p->isAlive = false;
                    close(p->fd);           // 这里检测关闭
                    LOG_F(INFO, "ip: %s\tport: %d\t lost connect for %d sec",
                          p->ip.c_str(), p->port, static_cast<int>(invalidInterval.count())); // 上一次心跳时间和现在比较
                } else {
                    sendAndRecv(p);
                }
                end:;//放锁
                waitGroup.Done();// 这个一定一定一定一定一定要注意 调用
            }
        });
    }
    waitGroup.Wait();
}

void KeepAlive::sendAndRecv(Participant *p) {
    // 心跳包的格式
    std::string keepAliveMsg{"GET ${alive}"};
    keepAliveMsg = Util::Encoder(keepAliveMsg);
    int len = send(p->fd, keepAliveMsg.c_str(), keepAliveMsg.size(), 0);
    if (len != keepAliveMsg.size()) {                    // 错误处理！！！
        LOG_F(ERROR, "ip: %s\tport: %d\tkeep alive send error", p->ip.c_str(), p->port);
        return;
    } else {
        LOG_F(INFO, "ip: %s\tport: %d\t send keepalive success!!!", p->ip.c_str(), p->port);
    }

    // 三秒钟给我回消息
    int rc = Util::recvByTime(p->fd, 3);
    if (rc < 0) {
        LOG_F(ERROR, "select error!!!! I dont know what happen");
        return;
    } else if (rc == 0) {
        LOG_F(INFO, "ip: %s\tport: %d\t dont reply??????????????", p->ip.c_str(), p->port);
    } else {
        char buf[BUFSIZ];  //数据传送的缓冲区
        len = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
        buf[len] = '\0';
        if (len <= 0) {     //这里是检测到关闭了， 上面是网络错误， 关闭了
            p->isAlive = false;
            close(p->fd);           // 这里检测关闭
            LOG_F(INFO, "ip: %s\tport: %d\t closed or shutdown", p->ip.c_str(), p->port);
            return;
        }
        LOG_F(INFO, "ip: %s\tport: %d\t at last reply me : %s", p->ip.c_str(),
              p->port, Util::outputProtocol(buf).c_str());
        p->lastAlive = std::chrono::system_clock::now();// 重新活了！！！
    }
}

// 活着的就不要传过来了！！！
bool KeepAlive::connectLostPa(Participant *p) {
    LOG_F(INFO, "trying to get connect with participant ip: %s\tport: %d", p->ip.c_str(), p->port);
    struct sockaddr_in remoteAddr{};
    int tmpSocket{};
    remoteAddr.sin_family = AF_INET; //设置为IP通信
    remoteAddr.sin_addr.s_addr = inet_addr(p->ip.c_str());//服务器IP地址
    remoteAddr.sin_port = htons(p->port); //服务器端口号
    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if ((tmpSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_F(INFO, "socket error");
    }
    if (connect(tmpSocket, (struct sockaddr *) &remoteAddr, sizeof(struct sockaddr)) < 0) {
        LOG_F(INFO, "ip: %s\tport: %d\t connect error", p->ip.c_str(), p->port);
    } else {
        p->fd = tmpSocket;    //更换 fd
        p->isAlive = true;
        p->lastAlive = std::chrono::system_clock::now();
        p->isBack = true; // 通知同步数据， 我是新来的， 要重新更新
        LOG_F(INFO, "participant ip: %s\tport: %d  come back", p->ip.c_str(), p->port);
        return true;
    }
    return false;
}
