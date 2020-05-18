//
// Created by Firefly on 2020/5/17.
//

#ifndef LAB3_PASERVER_H
#define LAB3_PASERVER_H

#include <string>
#include <utility>
#include <netinet/in.h>
#include <atomic>

/**
 * 设计思路：
 * 每次只保持一个连接，当有另一个连接到来时， 直接不理上一个连接
 * 一个线程用于
 */

class PaServer {

public:
    PaServer() {}

    PaServer(std::string ip, int port) : ip(std::move(ip)), port(port) {}

    PaServer &init();
    void setCoordinator(std::string ip, int port);

    void run();


private:
    int port;
    std::string ip;
    int serverSockfd;
    struct sockaddr_in serverAddr;   //服务器网络地址结构体

    std::atomic<int> counter{0}; // 记录有多少个 co 连接了， 理论只有一个 co

};


#endif //LAB3_PASERVER_H
