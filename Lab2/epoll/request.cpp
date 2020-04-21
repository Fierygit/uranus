//
// Created by Firefly on 2020/4/18.
//

#include "request.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <sys/socket.h>
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
    char response[BUFF_SIZE*10]={0};
    char *filename=get_filename();
    char *method=get_method();
    if(method==(char*)"GET"){//GET方法
        if(strlen(filename)==0){
            filename=(char*)"index.html";
        }
        // printf("文件名是: %s\n",filename);
        //响应
        char*mine;
        if(strstr(filename,".html"))
            mine=(char*)"text/html";
        else if(strstr(filename,".jpg"))
            mine=(char*)"image/jpeg";
        int filefd=open(filename,O_RDONLY);
        // printf("filefd: %d\n",filefd);
        sprintf(response,"HTTP/1.1 ");
        if(filefd<0){//不存在该文件
            sprintf(response+strlen(response),"404 NOT FOUND\r\nContent-Type:%s\r\n\r\n",mine);
            filefd=open("404.html",O_RDONLY);
        }else{
            sprintf(response+strlen(response),"200 OK\r\nContent-Type:%s\r\n\r\n",mine);
        }
        int headlen=strlen(response);
        int filelen=read(filefd,response+headlen,sizeof(response)-headlen);
        write(fd,response,strlen(response));
        close(filefd);
    }else if(method==(char*)"POST"){
        // printf("POST METHOD--------------------------\n");
    }else{
        sprintf(response,"HTTP/1.1 505 ERROR\r\n\r\n");
        write(fd,response,strlen(response));
    }
}
char* httpt_request::get_filename(){
    return (char*)("index.html");
}
char* httpt_request::get_method(){
    return (char*)("GET");
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
        std::cout << this->buf << std::endl;
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