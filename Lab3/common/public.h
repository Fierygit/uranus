//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_PUBLIC_H
#define LAB3_PUBLIC_H

#include <chrono>

enum OP {
    GET, SET, DEL
};

//LOG_F(INFO, "%d\t%s\t%s", command.op, command.key.c_str(), command.value.c_str());

struct Command {
    OP op;
    std::string key;
    std::string value;
};

struct Participant {

    std::string ip;
    int port;
    int fd;
    bool isAlive;

    Participant(std::string ip, int port) : ip(std::move(ip)), port(port) {}
};

typedef std::vector<Participant> Participants;
//using Participants = std::vector<Participant>;

using Time = decltype(std::chrono::system_clock::now());

#endif //LAB3_PUBLIC_H
