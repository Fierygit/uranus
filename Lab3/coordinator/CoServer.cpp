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
#include <fstream>

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


/**
 * 首先先连接 所有的 pa
 */

void CoServer::initPaSrver() {
/*
    // 从配置文件中读取数据， 然后去连接客户端
    std::string filePath{R"(D:\college3\cloud_computing\uranus\Lab3\coordinator\config.conf)"};
    std::ifstream file;
    file.open(filePath, std::ios::in);

    if (!file.is_open()) {
        LOG_F(ERROR, "can not open file");
        exit(1);//直接退出
    }
    std::string strLine;
    while (getline(file, strLine)) {
        if (strLine.empty())
            continue;
        LOG_F(INFO, "participant info: %s", strLine.c_str());
        for (int i = 0; i < strLine.size(); i++) {
            if (strLine[i] == ':' && i != strLine.size() - 1 && i != 0) {
                std::string ip = strLine.substr(0, i + 1);
                std::string port = strLine.substr(i + 1);
                LOG_F(INFO, "split -> ip: %s  port: %d", ip.c_str(), atoi(port.c_str()));
                this->participants.push_back(Participant(ip, atoi(port.c_str())));
                break;
            }
        }
    }
*/

 // 上面已经连接好了所有的 连接， 这里尝试去连接多有的 participant, 连接好后，所有都保存在这里

    for(Participant &p : this->participants){

//        memset(&remoteAddr, 0, sizeof(remoteAddr)); //数据初始化--清零
//
//        remoteAddr.sin_family = AF_INET; //设置为IP通信
//        remoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
//        remoteAddr.sin_port = htons(8888); //服务器端口号
//
//        /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
//        if ((clientSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//            perror("socket error");
//            exit(1);
//        }
//
//        /*将套接字绑定到服务器的网络地址上*/
//        if (connect(clientSockfd, (struct sockaddr *) &remoteAddr, sizeof(struct sockaddr)) < 0) {
//            perror("connect error");
//            exit(1);
//        }

    }

}

/**
 * 将发送数据放入线程池中， 用 waitgroup 等待， 当接受到所有的返回时， 处理返回情况
 *
 */

void CoServer::send2PaSync() {

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
    std::thread acThread{[this] { clientAcceptHandler(this); }};
    acThread.detach();

    /*
     * 同步连接 所有的 participant, 连接不到算初始话失败
     */
    initPaSrver();
    LOG_F(INFO,"init over");
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






