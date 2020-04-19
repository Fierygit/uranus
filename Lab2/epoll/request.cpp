//
// Created by Firefly on 2020/4/18.
//

#include "request.h"
#include <unistd.h>
#include "sys/epoll.h"
#include <iostream>

bool clients_pool::add_one(httpt_request *request) {
    this->fd2request[request->fd] = request;
    return false;
}

httpt_request *clients_pool::get_by_fd(int client_sockt) {
    return fd2request[client_sockt];
}

bool httpt_request::is_static() {
    return this->is_sta;
}

void httpt_request::response_static() {
}

void httpt_request::get_buff(int& epfd) {
    char buff[BUFF_SIZE];
    int str_len = read(this->fd, buff, BUFF_SIZE);
    printf("str_len: %d\n", str_len);
    if (str_len > 0) {  //echo to client
        buff[str_len] = 0;
        for (int i = 0; i < str_len; i++) {
            this->buf += buff[i];
        }
        std::cout << this->buf << std::endl;
        // 可以返回了解析了
        this->is_sta = true;
//        printf("Message from client %d: %s", this->fd, buff);
//        write(this->fd, buff, str_len);  //echo!
    } else {                                         //close connection
        epoll_ctl(epfd, EPOLL_CTL_DEL, this->fd, NULL);
        close(this->fd);
        printf("Disconnected client %d!\n", this->fd);
    }
}