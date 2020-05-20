//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_PUBLIC_H
#define LAB3_PUBLIC_H

#include <chrono>
#include <vector>
#include <mutex>

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
    RequestReply pc1Reply{0, ""};
    RequestReply pc2Reply{0, ""};
};

typedef std::vector<Participant *> Participants;
//using Participants = std::vector<Participant>;


#endif //LAB3_PUBLIC_H
