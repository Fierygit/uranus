//
// Created by Firefly on 2020/5/10.
//


#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "Client.h"
#include "../common/public.h"
#include "../common/loguru.hpp"
#include "../common/Util.h"


Client *Client::init() {
    //欢迎logo
    //todo
    //std::cout << welcomeLogo << std::endl;

    // remoteAddr{} 也是 0
    memset(&remoteAddr, 0, sizeof(remoteAddr)); //数据初始化--清零

    remoteAddr.sin_family = AF_INET; //设置为IP通信
    remoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
    remoteAddr.sin_port = htons(8001); //服务器端口号

    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if ((clientSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    /*将套接字绑定到服务器的网络地址上*/
    if (connect(clientSockfd, (struct sockaddr *) &remoteAddr, sizeof(struct sockaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    return this;
}

void Client::run() {

    // 测试， 收服务端的消息
    char buf[BUFSIZ];  //数据传送的缓冲区
//    printf("connected to server\n");
    int len = recv(clientSockfd, buf, BUFSIZ, 0);//接收服务器端信息
    buf[len] = '\0';
    std::cout << buf << std::endl;

    // 同步执行
    for (;;) {
        std::cout << "uranus client > ";
        std::string line;
        getline(std::cin, line);
        // std::cout << line << std::endl; //ceshi
        std::string sendMsg = Util::Encoder(line);
        if (sendMsg != "null") sendToServer(sendMsg);
        else std::cout << "Error!!" << std::endl;
    }

}

// 不处理出错问题
void Client::sendToServer(std::string msg) {

    // 所有的 send 都要 try catch
    int len = send(clientSockfd, msg.c_str(), msg.size(), 0);
//    std::cout << len << std::endl;
    if (len != msg.size()) {
        // 错误处理！！！
        std::cout << "Error**************************" << std::endl;
    } else {
        std::cout << "ok" << std::endl;
    }
    // 等待接收消息，收到了才可以处理下一条消息

    char buf[BUFSIZ];  //数据传送的缓冲区
    len = recv(clientSockfd, buf, BUFSIZ, 0);//接收服务器端信息
    std::cout << len << std::endl;
    buf[len] = '\0';
    std::cout << buf << std::endl;
}

