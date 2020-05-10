//
// Created by Firefly on 2020/5/10.
//

#include <syslog.h>
#include <thread>
#include "CoServer.h"
#include "../common/public.h"
#include "../common/loguru.hpp"

void CoServer::run() {

}

CoServer CoServer::init() {
    std::cout << welcomeLogo << std::endl;
    LOG_F(INFO,"addr: %s, port: %d",addr.c_str());

    auto func = []() {
        std::cout << "hi" << std::endl;
    };
    std::thread t{func};
    t.detach();
    return *this;
}

