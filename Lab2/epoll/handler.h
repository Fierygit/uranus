//
// Created by Firefly on 2020/4/18.
//
#include "ThreadPool.h"
#include "request.h"


class http_handler {
public:
    http_handler();

public:
    bool dispatch(struct epoll_event &event, int listen_fd, int epfd);

private:

    uranus::ThreadPool *thread_pool;
    clients_pool* clients;

};