//
// Created by Firefly on 2020/4/17.
//
<<<<<<< HEAD



=======
// 编译的时候提示 不使用 <bits/socket.h> 这个而使用 #include <sys/socket.h> 这个了
//#include <bits/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
>>>>>>> master
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include "sys/epoll.h"
#include "debug_log.h"
#include "handler.h"
#include <arpa/inet.h>
#include <fcntl.h>

class http_server {
public:
    http_server();

    http_server(uint16_t server_port);
    ~http_server(); //创建完，为什么自动析构？？？
    void start();

private:
    void init();

    void init_epoll();

    void init_socket();

    void init_others();

private:

    int serv_sock;
    uint16_t server_port;           // 端口号是 16 位的， 最多支持 2^16 次方的端口 大约几万个
    std::string server_ip = "127.0.0.1";
    sockaddr_in server_addr{}; // 自动初始化,包括分配空间 {}， c11 的特性，一定要， 这是一个结构体

private:
    int epfd;
    int epoll_size = 1000; // 直接在这里改了
    epoll_event *events;
    bool enable_et = false;

private:
    http_handler *handler;
};