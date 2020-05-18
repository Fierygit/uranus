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
            for (;;) {
                char buf[BUFSIZ];  //数据传送的缓冲区
                int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
                buf[len] = '\0';

                if (len <= 0) { // 如果co 挂了
                    LOG_F(WARNING, "connection closed!!!");
                    this->counter--;
                }
                std::cout << buf << std::endl;
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

