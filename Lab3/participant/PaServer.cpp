//
// Created by Firefly on 2020/5/17.
//

#include "PaServer.h"
#include "../common/loguru.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <thread>

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
        std::thread tmp{[this, clientSocket] { // 不要用引用， clientSocket 是局部变量
            char buf[BUFSIZ];  //数据传送的缓冲区
            // 0 : 普通情况, 可以执行
            // 1 : 进入了1phase, 等待1phase
            int status = 0;
            for (;;) {
                LOG_F(INFO, "waiting for recv ...");
                int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
                buf[len] = '\0';
                LOG_F(INFO, "recv success .");
                LOG_F(INFO, "recv msg: %s", buf);

                if (status == 0) {  // 1phase之前, 可以处理
                    // 首先判断命令是否合法(如是否是 2phase命令, 如果是那么抛弃)

                    // 合法返回成功
                    std::string send_msg = "success";
                    if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
                        LOG_F(ERROR, "part send error !");
                    }

                    if (len <= 0) { // 如果co 挂了
                        LOG_F(WARNING, "connection closed!!!");
                        this->counter--;
//                    break;
                    }
                    status = 1;
                    LOG_F(INFO, "status: 0 -> 1");
                } else if (status == 1) {  // 如果已经进入了 1phase, 等待2phase
                    // 首先判断命令是否合法

                    // 合法返回成功
                    // 合法返回成功
                    std::string send_msg = "success";
                    if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
                        LOG_F(ERROR, "part send error !");
                    }

                    if (len <= 0) { // 如果co 挂了
                        LOG_F(WARNING, "connection closed!!!");
                        this->counter--;
//                    break;
                    }
                    status = 0;
                    LOG_F(INFO, "status: 1 -> 0");
                }

            }
        }};
        tmp.detach();
    }
    LOG_F(INFO, "should not run here!!!");
}

// 设置该参与者所属协调者, 但其实不用设置? 因为它只需要等待协调者跟它连接, 他不主动连接
void PaServer::setCoordinator(std::string ip, int port) {
    // 不需要设置
    return;
}

