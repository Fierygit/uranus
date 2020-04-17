//
// Created by Firefly on 2020/4/18.
//

#include <sys/epoll.h>
#include "handler.h"

bool http_handler::dispatch(struct epoll_event event, int listen_fd, int epoll_fd) {
    return false;
}

