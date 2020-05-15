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
#include "../common/Util.h"
#include <arpa/inet.h>

//todo 放到 public 重复包含？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？

std::string welcomeLogo = "             _  .-')     ('-.         .-') _               .-')    \n"
                          "            ( \\( -O )   ( OO ).-.    ( OO ) )             ( OO ).  \n"
                          " ,--. ,--.   ,------.   / . --. /,--./ ,--,' ,--. ,--.   (_)---\\_) \n"
                          " |  | |  |   |   /`. '  | \\-.  \\ |   \\ |  |\\ |  | |  |   /    _ |  \n"
                          " |  | | .-') |  /  | |.-'-'  |  ||    \\|  | )|  | | .-') \\  :` `.  \n"
                          " |  |_|( OO )|  |_.' | \\| |_.'  ||  .     |/ |  |_|( OO ) '..`''.) \n"
                          " |  | | `-' /|  .  '.'  |  .-.  ||  |\\    |  |  | | `-' /.-._)   \\ \n"
                          "('  '-'(_.-' |  |\\  \\   |  | |  ||  | \\   | ('  '-'(_.-' \\       / \n"
                          "  `-----'    `--' '--'  `--' `--'`--'  `--'   `-----'     `-----'  ";

void CoServer::run() {
    for (;;) {
        /**
         * 阻塞接受， 所有 client 的请求， 只要有请求就处理， 每次处理一个
         */
        TaskNode taskNode = this->tastNodes->get();
        Client client = taskNode.first;
        Command command = taskNode.second;
        LOG_F(INFO, "OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(), command.value.c_str());
        sockaddr_in clientAddr = client.addr;

        // 先假设 2pc 必定成功
        if (true) {
            std::string rep{"SUCCESS!"};
            if (send(client.fd, rep.c_str(), rep.length(), 0) < 0) {
                LOG_F(ERROR, "client is bad : %s", inet_ntoa(clientAddr.sin_addr));
                return;
            }
        }
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

const CoServer::Clients &CoServer::getClients() const {
    return clients;
}

void CoServer::addClient(Client client) const {

}

BoundedBlockingQueue<CoServer::TaskNode> *CoServer::getTastNodes() const {
    return this->tastNodes;
}



