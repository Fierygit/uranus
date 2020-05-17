//
// Created by Firefly on 2020/5/17.
//

#include "PaServer.h"
#include "../common/loguru.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <thread>

PaServer &PaServer::init() {
    std::cout << "请输入端口号: \n";
    std::cin >> port;

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

    LOG_F(INFO, "start to listen !!!");
    return *this;
}

void PaServer::run() {

    std::thread t{[]{}};

    for (;;) {}

}

