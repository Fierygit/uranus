//
// Created by Firefly on 2020/5/17.
//

#include "PaServer.h"
#include "../common/loguru.hpp"
#include "../common/Util.h"
#include "../common/public.h"
#include <arpa/inet.h>
#include <iostream>
#include <thread>

/* NOTE:
 * 协议: 协调者返回的数据格式
 * 对于1p, 协调者返回SET STATUS "0" / "1" 代表 1phase 成功或失败
 * 对于2p, 协调者返回SET STATUS "0" 代表操作失败
 *      返回 SET ${val} XXX 代表 get的结果
 *      返回 SET STATUS "1" 代表set或del成功
 */

const int PRE_PHASE1 = 0;
const int AFTER_PHASE1 = 1;

PaServer &PaServer::init() {
    LOG_F(INFO, ":ip %s, port: %d", ip.c_str(), port);

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

    LOG_F(INFO, "start to listen ...");
    return *this;
}

void PaServer::run() {
    // 创建一个新的连接，接受请求， 当有一个
    for (;;) {
        struct sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        int clientSocket;
        if ((clientSocket = accept(serverSockfd, (struct sockaddr *) &clientAddr,
                                   reinterpret_cast<socklen_t *>(&addrLen))) < 0) {
            // 这里出错了不能 停止服务， 会是什么错误呢？??? //todo how to deal？？？？？？？？？？？？？
            LOG_F(ERROR, "accept error");
            continue;
        }
        LOG_F(INFO, "a new client is arrive : %s", inet_ntoa(serverAddr.sin_addr));
        this->counter++;
        // 注意格式
        std::thread handle(&PaServer::handleCoor, this, clientSocket);
        handle.detach();
    }
    LOG_F(INFO, "should not run here!!!");
}

// 设置该参与者所属协调者, 但其实不用设置? 因为它只需要等待协调者跟它连接, 他不主动连接
void PaServer::setCoordinator(std::string ip, int port) {
    // 不需要设置
    return;
}

// 处理协调者的通信
void PaServer::handleCoor(int clientSocket) {
    char buf[BUFSIZ];  //数据传送的缓冲区
    // 0 : 普通情况, 可以执行
    // 1 : 进入了1phase, 等待1phase
    int status = 0;
    Command command_1phase;  // 接收的命令
    for (;;) {
        LOG_F(INFO, "waiting for recv ...");
        int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
        buf[len] = '\0';
        LOG_F(INFO, "recv success .");
        LOG_F(INFO, "recv msg: %s", buf);
        // 一定要先处理
        if (len <= 0) { // 如果co 挂了
            LOG_F(WARNING, "connection closed!!!");
            this->counter--;
            break;
        }

        Command command = Util::Decoder(buf);
        LOG_F(INFO, "get command from queue OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(),
              command.value.c_str());

        std::string send_msg;

        // 1phase之前, 可以处理
        if (status == PRE_PHASE1) {
            // 首先判断命令是否合法(如是否是 2phase命令, 如果是那么抛弃)
            if (command.op < 0 || command.op > 2) {
                send_msg = "SET STATUS \"0\"";
            } else {
                // 合法返回成功
                send_msg = "SET STATUS \"1\"";
            }
            command_1phase = command;
        } else if (status == AFTER_PHASE1) {  // 如果已经进入了 1phase, 等待2phase
            // 首先判断命令是否合法
            std::string val;
            if (command.op == 1 && command.key == "${key}" && command.value == "${commit}") {
                // 要求提交
                if (command_1phase.op == SET) {  // SET == 1
                    KVDB[command_1phase.key] = command_1phase.value;
                    send_msg = "SET STATUS \"1\"";
                } else if (command_1phase.op == GET) {  // GET == 0
                    if (KVDB.count(command_1phase.key) == 0) {
                        // 没有这个值
                        send_msg = "SET STATUS \"0\"";
                    } else {
                        val = KVDB[command_1phase.key];
                        send_msg = "SET ${val} " + std::string("\"" + val + "\"");
                    }
                } else if (command_1phase.op == DEL) {  // DEL == 2
                    KVDB.erase(command_1phase.key);
                    send_msg = "SET STATUS \"1\"";
                } else {  // 命令不合法
                    send_msg = "SET STATUS \"0\"";
                }
            } else {
                // 要求终止
                send_msg = "SET STATUS \"0\"";
            }
        }
        LOG_F(INFO, "to send: %s", send_msg.c_str());
        // 统一发送
        send_msg = Util::Encoder(send_msg);
        LOG_F(INFO, "to send: %s", Util::outputProtocol(send_msg).c_str());
        if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
            LOG_F(ERROR, "part send error !");
        }

        if (status == PRE_PHASE1) {
            status = AFTER_PHASE1;
            LOG_F(INFO, "status: PRE_PHASE1 -> AFTER_PHASE1");
        } else if (status == AFTER_PHASE1) {
            status = PRE_PHASE1;
            LOG_F(INFO, "status: AFTER_PHASE1 -> PRE_PHASE1");
        }

    }
}

