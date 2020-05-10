//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_COSERVER_H
#define LAB3_COSERVER_H

#include <iostream>
#include <utility>
#include <vector>
#include <netinet/in.h>

/*
 * 连接的用户
 */

struct Client {
    int fd; // socket 的文件描述符
    sockaddr_in addr;
    bool isAlive;
};

struct Participant {

private:
    int fd;
    bool isAlive;

};

class CoServer {

public:

    CoServer() : CoServer("localhost", 8888) {}

    // 初始化地址就够了
    CoServer(std::string ip, int port) : port(8888), ip(std::move(ip)) {}

private:

    void initCoSrver();

public:
    CoServer init();
    void run();

    int getServerSockfd() const;

/*
 * 系统信息
 */

private:
    using Clients = std::vector<Client>;
    using Participants = std::vector<Participant>;

    Clients clients;
    Participants participants;

/*
 * 本服务器的信息
 */
private:
    int port;
    std::string ip;
    int serverSockfd;
    struct sockaddr_in serverAddr;   //服务器网络地址结构体


};


#endif //LAB3_COSERVER_H
