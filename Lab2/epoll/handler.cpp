//
// Created by Firefly on 2020/4/18.
//

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "handler.h"
#include "debug_log.h"
#include "unistd.h"

void check_event_type(uint32_t);

http_handler::http_handler() : thread_pool(new uranus::ThreadPool()), clients(new clients_pool()) {}


/**
 * epfd 是为了 关闭的
 * @param event
 * @param serv_sock
 * @param epfd
 * @return
 */
bool http_handler::dispatch(epoll_event &event, int serv_sock, int epfd) {

    // 创建 新的连接, 封装成一个request
    return this->thread_pool->addTask([&event, &serv_sock, &epfd, this]() {
        check_event_type(event.events);
        sockaddr_in clnt_adr{};
        if (event.data.fd == serv_sock) {
            struct sockaddr_in client_address{};
            socklen_t client_addrLength = sizeof(struct sockaddr_in);
            int client_sock = accept(serv_sock, (struct sockaddr *) &client_address, &client_addrLength);
            epoll_event event{};  //!!! 初始化｛｝
            //往内核事件表里添加事件
            event.events = EPOLLIN;
            event.data.fd = client_sock;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111
            epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);  // fuck ! 这里是 client_sock!!!
            fcntl(epfd, F_SETFL, fcntl(epfd, F_GETFD, 0) | O_NONBLOCK);

            auto *request = new httpt_request(client_sock);  // 要传递这个进去
            this->clients->add_one(request);
            DEBUG_LOG("a new client come: addr: %d, port: %d",
                      inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        } else {
            auto request = this->clients->get_by_fd(event.data.fd);
            request->get_buff(epfd);

            if (request->is_static()) {
                // TODO: 解析

                // TODO: 响应
                request->response_static();
            }
        }
        return true;

    });

}


void check_event_type(uint32_t type) {
    if (type & EPOLLIN)
        DEBUG_LOG("type check:\tEPOLLIN");
    if (type & EPOLLOUT)
        DEBUG_LOG("type check:\tEPOLLOUT");
    if (type & EPOLLRDHUP)
        DEBUG_LOG("type check:\tEPOLLRDHUP ");
    if (type & EPOLLPRI)
        DEBUG_LOG("type check:\tEPOLLPRI");
    if (type & EPOLLERR)
        DEBUG_LOG("type check:\tEPOLLERR");
    if (type & EPOLLHUP)
        DEBUG_LOG("type check:\tEPOLLHUP");
    if (type & EPOLLET)
        DEBUG_LOG("type check:\tEPOLLET");
    if (type & EPOLLONESHOT)
        DEBUG_LOG("type check:\tEPOLLONESHOT ");
    if (type & EPOLLWAKEUP)
        DEBUG_LOG("type check:\tEPOLLWAKEUP ");
}