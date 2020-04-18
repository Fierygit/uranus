//
// Created by Firefly on 2020/4/18.
//

class http_handler{

public:
    static bool dispatch(struct epoll_event event, int listen_fd, int epoll_fd);
};