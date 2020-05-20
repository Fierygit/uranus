//
// Created by Firefly on 2020/5/10.
//


#include <thread>
#include <cstring>
#include "CoServer.h"
#include "../common/public.h"
#include "../common/loguru.hpp"
#include "clientHandler.h"
#include "../common/Util.h"
#include "../common/WaitGroup.h"
#include <arpa/inet.h>

void CoServer::run() {
    for (;;) {
        // loguru::shutdown();
        /**
         * 阻塞接受， 所有 client 的请求， 只要有请求就处理， 每次处理一个
         */
        TaskNode taskNode = this->tastNodes->get();
        Client client = taskNode.first;
        std::string commandStr = taskNode.second;
        Command command = Util::Decoder(commandStr);
        sockaddr_in clientAddr = client.addr;

        LOG_F(INFO, "get command from queue OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(),
              command.value.c_str());

        int tmpIsAlive = 0;// 一个都没活着， 直接返回错误的
        for (Participant *&p : participants) if (p->isAlive)tmpIsAlive++;
        if (tmpIsAlive == 0) {
            std::string rep{"ERROR!"};
            if (send(client.fd, rep.c_str(), rep.length(), 0) < 0) {
                LOG_F(ERROR, "client is bad : %s", inet_ntoa(clientAddr.sin_addr));
                continue;// 最好代码可以复用， 先放着
            }
        }

        //################      stop the world      ############################################
        if (this->needSyncData) {
            LOG_F(INFO, "start sync the data");
            //
        }

        int pc1 = 0;
        // 1、 第一阶段，提交请求命令给particitans**************************************************
        // todo  万一 abort 的时候， 有机子断了导致数据不同步？？？

        this->send2PaSync(commandStr);// 同步发送------------------------------------------------
        LOG_F(INFO, "1 phase send over !");
        for (Participant *p : participants) {
            if (!p->isAlive) continue;
            if (p->pc1Reply.stateCode != SUCCESS) {
                pc1 = 1;
                break;
            }
        }

        int pc2 = 0;
        //2、 第二阶段*****************************************************************************
        if (pc1 == 0) {

            LOG_F(INFO, "1 phase success ! start to 2 phase");
            std::string msg = Util::Encoder("SET ${key} \"${commit}\"");
            LOG_F(INFO, "2 phase msg: %s", Util::outputProtocol(msg).c_str());

            this->send2PaSync(msg);// 同步发送------------------------------------------------------
            for (Participant *p : participants) {
                if (p->pc1Reply.stateCode != SUCCESS) {
                    pc2 = 1;
                }
            }
        } else {
            std::string msg = Util::Encoder("SET ${key} \"${abort}\"");
            this->send2PaSync(msg);
            pc2 = 1;
        }
        if (pc2 == 0) {
            std::string rep{"SUCCESS!"};
            if (send(client.fd, rep.c_str(), rep.length(), 0) < 0) {
                LOG_F(ERROR, "client is bad : %s", inet_ntoa(clientAddr.sin_addr));
                return;
            }
        } else {
            std::string rep{"ERROR!"};
            if (send(client.fd, rep.c_str(), rep.length(), 0) < 0) {
                LOG_F(ERROR, "client is bad : %s", inet_ntoa(clientAddr.sin_addr));
                return;
            }
        }
    }
}


/**
 * 首先先连接 所有的 pa, 当cor 故障重连的时候， 这里去恢复连接
 *
 * todo 断了恢复，要不要检查同步？？？
 */

void CoServer::initPaSrver() {
    LOG_F(INFO, "start to init PaServer ...");

    // 上面已经连接好了所有的 连接， 这里尝试去连接多有的 participant, 连接好后，所有都保存在这里

    // 确保每个线程执行完
    WaitGroup waitGroup;
    waitGroup.Add(participants.size());

    for (Participant *&p : this->participants) {
        // 放进线程池处理
        threadPool->addTask([this, &p, &waitGroup] { // 不要用引用， clientSocket 是局部变量
            struct sockaddr_in remoteAddr{}; //服务器端网络地址结构体
            int clientSockfd;

            memset(&remoteAddr, 0, sizeof(remoteAddr)); //数据初始化--清零

            remoteAddr.sin_family = AF_INET; //设置为IP通信
            remoteAddr.sin_addr.s_addr = inet_addr(p->ip.c_str());//服务器IP地址
            remoteAddr.sin_port = htons(p->port); //服务器端口号

            /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
            if ((clientSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket error");
                exit(1);
            }

            /*将套接字绑定到服务器的网络地址上*/
            if (connect(clientSockfd, (struct sockaddr *) &remoteAddr, sizeof(struct sockaddr)) < 0) {
                LOG_F(ERROR, "connect error... ip: %s, port: %d", p->ip.c_str(), p->port);
                p->fd = -1;
                p->isAlive = false;
            } else {
                LOG_F(INFO, "connect success... ip: %s, port: %d, socket: %d", p->ip.c_str(), p->port, clientSockfd);
                p->fd = clientSockfd;
                p->isAlive = true;
//                this->paNum++;
            }
            waitGroup.Done(); // 结束喽！！！！！！
        });
    }
    waitGroup.Wait();

    WaitGroup waitSyncGroup;
    waitSyncGroup.Add(this->getAliveCnt());
    std::vector<int> result(participants.size());
    int idx = -1;
    for (Participant * p: participants) {
        idx++;
        if (!p->isAlive) continue;
        // TODO: 一个编译错误, 为什么不可以这样调用??????? fuck!!!
        std::thread handleSync(&CoServer::getLatestIndex, this, p, &waitSyncGroup, idx, result);
        handleSync.detach();
    }
    waitSyncGroup.Wait();
}

/**
 * 将发送数据放入线程池中， 用 waitgroup 等待， 当接受到所有的返回时， 处理返回情况
 *
 */

// 发消息，返回的结果
void CoServer::send2PaSync(std::string msg) {
    LOG_F(INFO, "before msg send");
    WaitGroup waitGroup;
    int alive_cnt = 0;
    for (auto p: participants) {
        if (p->isAlive) {
            alive_cnt += 1;
        }
    }


    // 如果发送不了，直接返回错误， 然后执行abort
    waitGroup.Add(alive_cnt);//等待每一个 参与者的 到来
    for (Participant *&p : participants) {
        if (!p->isAlive) continue;
        this->threadPool->addTask([&] {//
            {// 锁的作用域, RAII
                p->pc1Reply = RequestReply{0, ""};// 清空,默认就是成功， 没有返回就是最好的
                std::unique_lock<std::mutex> uniqueLock(p->lock);// 获取锁
                if (send(p->fd, msg.c_str(), msg.size(), 0) != msg.size()) {
                    p->pc1Reply.stateCode = 2; //发送失败
                    LOG_F(WARNING, "participant %d send error!!!", p->port);
                    goto end;
                } else {            //发送完等待接受
                    LOG_F(INFO, "participant %d send success. waiting for receive...", p->port);
                    char buf[BUFSIZ];  //数据传送的缓冲区
                    int len = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
                    buf[len] = '\0';
                    if (len <= 0) { // 如果co 挂了
                        LOG_F(WARNING, "participant %d connection closed!!!", p->port);
                        p->pc1Reply.stateCode = 1; //接受挂了
                        goto end;
                    }
                    //std::cout << buf << std::endl;
                    LOG_F(INFO, "receive: len: %d  %s", len, Util::outputProtocol(buf).c_str());
                    Command command = Util::Decoder(buf);
                    LOG_F(INFO, "receive %d OP: %d\tkey: %s\tvalue: %s", p->port,
                          command.op, command.key.c_str(), command.value.c_str());
                }
                end:;// 释放锁
                waitGroup.Done(); // 结束喽！！！！！！
            }
        });
    }
//     把下面这个注释掉就可以返回了, 否则没有返回值
    waitGroup.Wait();  //等待所有的结果
}


/*
 * 初始化的时候出错直接停止， 不算容错范围
 */

void CoServer::initCoSrver() {
    LOG_F(INFO, ":ip: %s, port: %d", ip.c_str(), port);

    serverAddr.sin_family = PF_INET;    //设置为IP通信
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if ((serverSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_F(ERROR, "socket error");
        exit(1);
    }

    //绑定地址
    if (bind(serverSockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <
        0) {
        LOG_F(ERROR, "bind error");
        exit(EXIT_FAILURE);
    }

    //监听
    if (listen(serverSockfd, SOMAXCONN) < 0) {
        LOG_F(ERROR, "listen error");
        exit(EXIT_FAILURE);
    }

    LOG_F(INFO, "start to listen !!!");
}

CoServer &CoServer::init() {


    initCoSrver();

    // 创建子线程接受新的 client 连接
    std::thread acThread{[this] { clientAcceptHandler(this); }};
    acThread.detach();

    // 同步连接 所有的 participant,
    initPaSrver();

    this->keepAlive->init(this->participants);

    LOG_F(INFO, "init over");
    return *this;
}

int CoServer::getServerSockfd() const {
    return serverSockfd;
}

BoundedBlockingQueue<CoServer::TaskNode> *CoServer::getTastNodes() const {
    return this->tastNodes;
}

// 添加 参与者
void CoServer::setParticipant(std::vector<std::pair<std::string, std::string>> &parts) {
    for (const auto &p: parts) {
        auto *tmp = new Participant();
        tmp->ip = p.first, tmp->port = atoi(p.second.c_str());
        LOG_F(INFO, "add participant(ip: %s, port: %s)", p.first.c_str(), p.second.c_str());
        this->participants.push_back(tmp);
    }
}


CoServer::~CoServer() {
    {
        LOG_F(INFO, "is over but just dont delete all source");
    }
}

int CoServer::getAliveCnt() {
    int wait_cnt = 0;
    for (const auto& p: participants) {
        if (p->isAlive) {
            wait_cnt ++;
        }
    }
    return wait_cnt;
}

void CoServer::test1(Participant *p) {
    std::cout << 1 << std::endl;
}

// 获取一个 p 最新的 操作(日志)索引
void CoServer::getLatestIndex(Participant* p, WaitGroup *waitGroup, int idx, std::vector<int> &result) {
    LOG_F(INFO, "Trying to get Lastest index");

//    waitGroup.Done();
}

