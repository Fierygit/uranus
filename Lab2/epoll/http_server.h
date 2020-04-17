//
// Created by Firefly on 2020/4/17.
//
//#include <bits/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <fcntl.h>
#include "sys/epoll.h"
#include "debug_log.h"
#include "handler.h"

class http_server {
public:
    http_server();

    http_server(uint16_t server_port);

    void start();
private:
    void init_epoll();

private:
    http_handler handler{};

    int serv_sock;
    uint16_t server_port;           // 端口号是 16 位的， 最多支持 2^16 次方的端口 大约几万个
    std::string server_ip = "127.0.0.1";
    sockaddr_in server_addr{}; // 自动初始化,包括分配空间 {}， c11 的特性一定要， 这是一个结构体

    int epfd;
    int epoll_size = 1000;
    epoll_event *events;
    epoll_event event{};

    bool enable_et = 0;
};