//
// Created by Firefly on 2020/5/12.
//

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "../common/public.h"
#include "../common/loguru.hpp"
#include "../common/Util.h"

int main() {
    std::cout << "hi" << std::endl;
    for (int i=0; i<100; i++) {
        std::cout << "uranus client > ";
        std::string line;
        getline(std::cin, line);
//        LOG_F(INFO,"%s",line.c_str());
        std::string sendMsg = Util::Encoder(line);
        if (sendMsg != "null") {
            std::cout<<sendMsg;
        } else {
            std::cout << "Error!!" << std::endl;
        }
    }
    return 0;
}