//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_COSERVER_H
#define LAB3_COSERVER_H

#include <iostream>
#include <utility>
#include <vector>
#include <netinet/in.h>
#include "../common/public.h"
#include "../coordinator/BoundedBlockingQueue.h"

/*
 * 连接的用户
 */

struct Client {
    int fd; // socket 的文件描述符
    sockaddr_in addr;
    bool isAlive;
    std::string buf;
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
    CoServer(std::string ip, int port) :
            port(8888), ip(std::move(ip)), tastNodes(new BoundedBlockingQueue<TaskNode>()) {}

private:

    void initCoSrver();

public:
    CoServer init();

    void run();

    int getServerSockfd() const;

/*
 * 系统信息
 */

public:
    using Clients = std::vector<Client>;
    using Participants = std::vector<Participant>;
    using TaskNode = std::pair<Client,Command>;

public:
    const Clients &getClients() const;

    void addClient(Client) const;

private:
    //把并行的强行转为 串行， 最多等待 n 个任务， 当大于时， 停止服务 important
    BoundedBlockingQueue<TaskNode> *tastNodes;
public:
    BoundedBlockingQueue<TaskNode> *getTastNodes() const;

private:
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
