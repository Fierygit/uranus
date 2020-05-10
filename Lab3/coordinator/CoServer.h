//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_COSERVER_H
#define LAB3_COSERVER_H

#include <iostream>
#include <utility>
#include <vector>

/*
 * 连接的用户
 */

struct Client {


private:
    int fd; // socket 的文件描述符
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

    CoServer(std::string addr, int port) : port(8888), addr(std::move(addr)) {}


public:
    CoServer init();

    void run();


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
    std::string addr;

};


#endif //LAB3_COSERVER_H
