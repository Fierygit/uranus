//
// Created by Firefly on 2020/5/10.
//
#include "participantHandler.h"
#include "../common/Util.h"


std::string handler2pc(std::string &commandStr, Participants &participants, uranus::ThreadPool *threadPool) {
    int pc1 = 0;
    // 1、 第一阶段，提交请求命令给 participants **************************************************************************
    // todo  万一 abort 的时候， 有机子断了导致数据不同步？？？
    Command command = Util::Decoder(commandStr);

    send2PaSync(commandStr, participants, threadPool);// 同步发送
    LOG_F(INFO, "1 phase send over !");
    for (Participant *p : participants) {
        if (!p->isAlive) continue;
        if (p->Reply.stateCode != SUCCESS) {
            pc1 = 1;
            break;
        }
    }

    int pc2 = 0;
    //2、 第二阶段***************************************************************************************************************
    if (pc1 == 0) {

        LOG_F(INFO, "1 phase success ! start to 2 phase");
        std::string msg = Util::Encoder("SET ${key} \"${commit}\"");
        LOG_F(INFO, "2 phase msg: %s", Util::outputProtocol(msg).c_str());

        send2PaSync(msg, participants,
                    threadPool);// 同步发送

        for (Participant *p : participants) {
            if (!p->isAlive) continue;
            if (p->Reply.stateCode != SUCCESS) {
                pc2 = 1;
            }
        }

        int tmpIsAlive = 0;// 一个都没活着， 直接返回错误的
        for (Participant *&p : participants) if (p->isAlive)tmpIsAlive++;
        if (tmpIsAlive == 0)        pc2 = 1;


    } else {
        std::string msg = Util::Encoder("SET ${key} \"${abort}\"");
        send2PaSync(msg, participants, threadPool);
        pc2 = 1;
    }

    // back ot client ----------------------------------------------------------------------------------------------------
    std::string rep;
    if (pc2 == 0) {
        for (auto &p : participants)                        // 首先获取 2pc 第二阶段的返回值
            if (p->isAlive) {
                rep = p->Reply.info;
                break;
            }

        std::cout << rep << std::endl;

        if (command.op == SET) rep = "+OK\r\n";           // SET------------
        else if (command.op == DEL) {                       // DEL------------
            if (rep == "0") rep = "-ERROR\r\n";
            else rep = ":" + rep + "\r\n";
        } else {                                             // GET-------------(不改变就是原值)
            if (command.op == GET && rep == "${NULL}") rep = "*1\r\n$3\r\nnil\r\n";
            else rep = Util::EncodeResult(rep);
        }
    } else rep = "-ERROR\r\n";

    return rep;
}









/**
 * 将发送数据放入线程池中， 用 waitgroup 等待， 当接受到所有的返回时， 处理返回情况
 *
 * 等待接收只会等待 3 秒， 已经固定在代码里面
 *
 */
// 发消息，返回的结果
void send2PaSync(std::string msg, Participants &participants, uranus::ThreadPool *threadPool) {
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
    for (Participant *p : participants) {
        if (!p->isAlive) continue;
        threadPool->addTask([p, &msg, &waitGroup] {//p一定要值引用,其它无所谓
            {// 锁的作用域, RAII
                p->Reply = RequestReply{0, ""};// 清空,默认就是成功， 没有返回就是最好的
                std::unique_lock<std::mutex> uniqueLock(p->lock);// 获取锁
                //发数据
                if (send(p->fd, msg.c_str(), msg.size(), 0) != msg.size()) {
                    p->Reply.stateCode = 2; //发送失败
                    LOG_F(WARNING, "participant %d send error!!!", p->port);
                    goto end;
                } else { //发送完等待接受
                    LOG_F(INFO, "participant %d send success. waiting for receive...", p->port);
                    int rc = Util::recvByTime(p->fd, 3); //设置定时器
                    if (rc < 0) {
                        LOG_F(ERROR, "select error!!!! I dont know what happen");
                        return;
                    } else if (rc == 0) { // 超时不鸟人
                        LOG_F(INFO, "ip: %s\tport: %d\t dont reply??????????????", p->ip.c_str(), p->port);
                        p->Reply.stateCode = 1; // abort 但是不挂pa
                    } else {
                        char buf[BUFSIZ];  //数据传送的缓冲区
                        int len = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
                        buf[len] = '\0';
                        if (len <= 0) { // 如果co 挂了
                            LOG_F(WARNING, "participant %d connection closed!!!", p->port);
                            p->Reply.stateCode = 1; //接受挂了
                            p->isAlive = false;
                            goto end;
                        }
                        LOG_F(INFO, "receive: len: %d  %s", len, Util::outputProtocol(buf).c_str());
                        Command command = Util::Decoder(buf);
                        LOG_F(INFO, "receive %d OP: %d\tkey: %s\tvalue: %s", p->port, command.op, command.key.c_str(), command.value.c_str());
                        p->Reply.info = command.value;
                    }
                }
                end:;// 释放锁
                waitGroup.Done(); // 结束喽！！！！！！
            }
        });
    }
    waitGroup.Wait();  //等待所有的结果
}