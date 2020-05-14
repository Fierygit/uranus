//
// Created by 陈杰 on 2020/5/14.
//
#include <iostream>
#include <string>
#include "../common/Util.h"
#include "../common/public.h"
#include <utility>
#include <vector>

std::vector<std::string> ve{
        "*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n",
        "*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nC",
        "*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n"
};

bool test(std::string str) {
    return (Util::HandleBanBao(std::move(str)));
}

int main() {
    //"*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n"
    for (const auto &it : ve) {
        if (test(it)) {
            std::cout << "不是半包\n";
        } else {
            std::cout << "是半包\n";
        }
    }
}

