//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_PUBLIC_H
#define LAB3_PUBLIC_H

#include <chrono>
#include <vector>
#include <mutex>
#include <arpa/inet.h>

#define ERROR_REP "-ERROR\\r\\n"


/*
 * 连接的用户
 */
struct Client {
    int fd; // socket 的文件描述符
    sockaddr_in addr;
    std::string buf;
};


using Clients = std::vector<Client>;
using TaskNode = std::pair<Client, std::string>;

enum OP {
    GET, SET, DEL
};

//LOG_F(INFO, "%d\t%s\t%s", command.op, command.key.c_str(), command.value.c_str());

struct Command {
    OP op;
    std::string key;
    std::string value;
};


// for participant
const int SUCCESS = 0;
const int CON_ERROR = 1;

struct RequestReply {
    int stateCode;
    std::string info;
};


using Time = decltype(std::chrono::system_clock::now());


// 用应用传递, 只能有一个地方使用 fd
struct Participant {
    std::mutex lock;
    std::string ip;
    int port;
    int fd;
    bool isAlive;
    bool isBack;
    int lastIndex;
    Time lastAlive{};
    RequestReply Reply{0, ""};
};

typedef std::vector<Participant *> Participants;
//using Participants = std::vector<Participant>;


#endif //LAB3_PUBLIC_H
