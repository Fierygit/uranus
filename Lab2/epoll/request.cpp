//
// Created by Firefly on 2020/4/18.
//

#include "request.h"
#include <unistd.h>
#include "sys/epoll.h"
#include "debug_log.h"
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
    DEBUG_LOG("str_len: %d", str_len);
    if (str_len > 0) {  //echo to client
        buff[str_len] = 0;
        for (int i = 0; i < str_len; i++) {
            this->buf += buff[i];
        }
        std::cout << this->buf;
        std::cout << "HTTP方法与url获取~~~"<< std::endl;
        if ( buf.empty() )
        {
            return ;
        }
// 行缓冲区
        char szLineBuf[256] = {0};
// 通过流来提取报文头中内容
        std::stringstream ssHeader(buf.c_str());
// 提取第一行内容
        ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
        char  szHttpVersion[256] = {0};
        char szHttpVersion1[256] = {0};
        sscanf(szLineBuf,  "%[^ ]", szHttpVersion);
        sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
        way = szHttpVersion;
        url = szHttpVersion1;
        std::cout<<"方法是:"<<std::endl<<way<<std::endl;
        std::cout<<"请求的url是:"<<std::endl<<url<<std::endl;
        std::cout << "解析结束~~~"<< std::endl<<std::endl;
        // 可以返回了解析了
        this->is_sta = true;
//        printf("Message from client %d: %s", this->fd, buff);
//        write(this->fd, buff, str_len);  //echo!
    } else {                                         //close connection
        epoll_ctl(epfd, EPOLL_CTL_DEL, this->fd, NULL);
        close(this->fd);
        DEBUG_LOG("Disconnected client %d!", this->fd);
    }
}