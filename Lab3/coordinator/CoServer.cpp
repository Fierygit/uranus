//
// Created by Firefly on 2020/5/10.
//

#include <syslog.h>
#include <thread>
#include "CoServer.h"
#include "../common/public.h"
#include "../common/loguru.hpp"

void CoServer::run() {
    for (;;) {
    }

}


CoServer CoServer::init() {
    std::cout << welcomeLogo << std::endl;
    LOG_F(INFO, "addr: %s, port: %d", addr.c_str());

    /*
     * 创建子线程接受新的 pantipant 请求
     */
    auto acceptParticipant = [this]() {
        std::cout << "hi" << std::endl;
    };
    std::thread paThread{acceptParticipant};
    paThread.detach();

    /*
     * 创建子线程接受新的 client 连接
     */
    auto acceptClient = [this]() {
        std::cout << "hi" << std::endl;
    };
    std::thread acThread{acceptParticipant};
    acThread.detach();


    return *this;
}

