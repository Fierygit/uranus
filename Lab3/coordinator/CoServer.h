//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_COSERVER_H
#define LAB3_COSERVER_H

#include <iostream>
#include <utility>
#include <vector>
#include <netinet/in.h>
#include <map>
#include <atomic>
#include "../common/public.h"
#include "../coordinator/BoundedBlockingQueue.h"
#include "KeepAlive.h"
#include "../common/ThreadPool.h"
#include "../common/WaitGroup.h"

/*
 * 连接的用户
 */

struct Client {
    int fd; // socket 的文件描述符
    sockaddr_in addr;
    bool isAlive;
    std::string buf;
};


class CoServer {

public:

    CoServer() : CoServer("localhost", 8888) {}

    // 初始化地址就够了
    CoServer(std::string ip, int port) :
            port(port),
            ip(std::move(ip)),
            tastNodes(new BoundedBlockingQueue<TaskNode>()),
            keepAlive(new KeepAlive(5, 11)),
            threadPool(new uranus::ThreadPool(3)),
            needSyncData(false) {}

    ~CoServer();

private:

    void initCoSrver();

    void initPaSrver();

    int getAliveCnt();

    void send2PaSync(std::string msg);


public:
    CoServer &init();

    // 设置参与者
    void setParticipant(std::vector<std::pair<std::string, std::string>> &);

    void run();

    int getServerSockfd() const;
    void getLatestIndex(Participant *p, WaitGroup* waitGroup, int idx, std::vector<int> &result);
    void test1(Participant *p);

/*
 * 系统信息
 */

public:
    using Clients = std::vector<Client>;
    using TaskNode = std::pair<Client, std::string>;

public:

    BoundedBlockingQueue<TaskNode> *getTastNodes() const;

private:
    //把并行的强行转为 串行， 最多等待 n 个任务， 当大于时， 停止服务 important
    BoundedBlockingQueue<TaskNode> *tastNodes;

    // 心跳检测
    KeepAlive *keepAlive;

    // 用于发送信息的，线程池
    uranus::ThreadPool *threadPool;

    std::atomic<bool> needSyncData;


private:
    Clients clients;
    Participants participants;
    //std::atomic<int> paNum; //活着多少个pa， 一个都没有返回false


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
