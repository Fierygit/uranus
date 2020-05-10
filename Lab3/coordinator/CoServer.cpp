//
// Created by Firefly on 2020/5/10.
//

#include <syslog.h>
#include <thread>
#include "CoServer.h"
#include "../common/public.h"
#include "../common/loguru.hpp"
#include "clientHandler.h"
#include "participantHandler.h"

void CoServer::run() {
    for (;;) {
    }

}

/*
 * 初始化的时候出错直接停止， 不算容错范围
 */

void CoServer::initCoSrver() {
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
}

CoServer CoServer::init() {
    //欢迎logo
    std::cout << welcomeLogo << std::endl;

    initCoSrver();

    /*
     * 创建子线程接受新的 client 连接
     */
    auto acceptClient = [this]() {
        clientAcceptHandler(this);
    };
    std::thread acThread{acceptClient};
    acThread.detach();

    /*
     * 创建子线程接受新的 pantipant 请求
     *
     */
    std::thread paThread{[this]() { participantHandler(this); }};
    paThread.detach();

    return *this;
}

int CoServer::getServerSockfd() const {
    return serverSockfd;
}


