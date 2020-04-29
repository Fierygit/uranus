#include <iostream>
#include "http_server.h"
#include <unistd.h>
#include <getopt.h>


int main(int argc, char *argv[]) {
    std::string ip = "127.0.0.1";
    int port = 8888;
    int numThread = 4;

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
                printf("port is %s\n", optarg);
                break;
            case 'i':
                ip = (std::string)optarg;
                printf("ip is %s\n", optarg);
                break;
            case 't':
                numThread = atoi(optarg);
                printf("number thread is %s\n", optarg);
                break;
            case '?':
                printf("unknown option\n");
                break;
            case 0:
                printf("the return val is 0\n");
                break;
            default:
                printf("------\n");
        }
    }

    auto *server = new http_server(ip, port, numThread);
    server->start();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
