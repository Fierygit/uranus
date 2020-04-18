//
// Created by Firefly on 2020/4/17.
//

// 找不到这个 zconf.h 文件
//#include <zconf.h>
#include "http_server.h"
#include "debug_log.h"
#include <unistd.h>

http_server::http_server() {
    http_server(8888); // 默认 8888 端口
}

http_server::http_server(uint16_t server_port) : server_port(server_port) { init_epoll();  }


void http_server::start() {
    while (true) {
        // epoll_events_count表示就绪事件的数目, 这里会循环等待， 直到有事件的到来
        int epoll_events_count = epoll_wait(epfd, events, epoll_size, -1);
        if (epoll_events_count < 0) {
            DEBUG_LOG("epoll failure");
            break;
        }

        DEBUG_LOG("epoll events count : %d", epoll_events_count);

        //处理这epoll_events_count个就绪事件
        for (int i = 0; i < epoll_events_count; ++i) {
            while (!(handler.dispatch(events[i], serv_sock, epfd))) {
                DEBUG_LOG("处理失败");
                sleep(10);
            }
            // dispatcher(events[i], listener, epfd);
        }
    }
}


void http_server::init_epoll() {
    /**
    * 1:创建套接字socket, 只是绑定一些基础的类型
    * param1:指定地址族为IPv4;param2:指定传输协议为流式套接字;param3:指定传输协议为TCP,可设为0,由系统推导
     * PF 是 posix AF 是 BSD
    */
    this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    /**
     * 设置 server 端的 协议， 端口， 和 地址，
     */
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /** 把socket 绑定在 一个端口上
     * 返回-1 代表失败
     */
    if (bind(serv_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) <
        0) {
        DEBUG_LOG("bind error");
        exit(1);
    }

    /**
     *  监听 绑定好的端口
     * */
    if (listen(serv_sock, SOMAXCONN) < 0) {
        DEBUG_LOG("bind error");
        exit(1);
    }

    //在内核中创建事件表
    epfd = epoll_create(epoll_size);
    if (epfd < 0) {
        DEBUG_LOG("epfd create error");
        exit(1);
    }
    DEBUG_LOG("epoll created, epollfd = %d", epfd);
    events = new epoll_event[epoll_size];  // 表示监听的事件类型

    //往内核事件表里添加事件
    /**
    * 将文件描述符 fd 添加到 epollfd 标示的内核事件表中,
    * 并注册 EPOLLIN 和 EPOOLET 事件,
    * EPOLLIN 是数据可读事件；EPOOLET 表明是 ET 工作方式。
    * 最后将文件描述符设置非阻塞方式
    * @param epoll_fd :epoll 文件描述符
    * @param fd      :文件描述符
    * @param enable_et:enable_et = true,
    * 是否采用epoll的ET(边缘触发)工作方式；否则采用LT(水平触发)工作方式
       */

    event.data.fd = epfd;
    event.events = EPOLLIN;

    //是否为 边界出发，
    if (enable_et) { // 默认不开启， 开启了 只触发一次
        event.events = EPOLLIN | EPOLLET;
    }
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    //设置为非阻塞 io
    fcntl(epfd, F_SETFL, fcntl(epfd, F_GETFD, 0) | O_NONBLOCK);
    DEBUG_LOG("epoll init over!!!");
}
