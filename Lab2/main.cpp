#include <iostream>
#include "http_server.h"
#include <unistd.h>
#include <getopt.h>


int main(int argc, char *argv[]) {
    // 参数默认值
    std::string ip = "127.0.0.1";
    int port = 8888;
    int numThread = 4;

    // 长短参数解析
    const char *optstring = "p:i:t:";
    int c;
    struct option opts[] = {
        {"port", 1, NULL, 'p'},
        {"ip", 1, NULL, 'i'},
        {"number-thread", 1, NULL, 't'}};

    while ((c = getopt_long(argc, argv, optstring, opts, NULL)) != -1) {
        switch (c) {
            case 'p':
                port = atoi(optarg);
                // DEBUG_LOG("port is %s\n", optarg);
                break;
            case 'i':
                ip = (std::string)optarg;
                // DEBUG_LOG("ip is %s\n", optarg);
                break;
            case 't':
                numThread = atoi(optarg);
                // DEBUG_LOG("number thread is %s\n", optarg);
                break;
            case '?':
                DEBUG_LOG("unknown option\n");
                break;
            case 0:
                DEBUG_LOG("the return val is 0\n");
                break;
            default:
                DEBUG_LOG("------\n");
        }
    }
    // ip等日志信息
    DEBUG_LOG(("ip \t\t: " + ip).c_str());
    DEBUG_LOG(("port \t: " + std::to_string(port)).c_str());
    DEBUG_LOG(("num thread \t: " + std::to_string(numThread)).c_str());
    DEBUG_LOG("\n");
    auto *server = new http_server(ip, port, numThread);
    server->start();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
