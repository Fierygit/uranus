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
    std::cout << "HTTP方法与url获取~~~"<< std::endl;
// 行缓冲区
    char szLineBuf[128] = {0};
    char httpbuff[256];
    strcpy(httpbuff, buf.c_str());
// 通过流来提取报文头中内容
    int sizeBuf= sizeof(buf);
    std::cout<<"测试1"<<sizeBuf<<std::endl
    <<buf[sizeBuf-4]<<buf[sizeBuf-3]<<buf[sizeBuf-2]<<buf[sizeBuf-1]<<"测试2";
    std::stringstream ssHeader(this->buf.c_str());
// 提取第一行内容
    ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
    char  szHttpVersion[128] = {0};
    char szHttpVersion1[128] = {0};
    sscanf(szLineBuf,  "%[^ ]", szHttpVersion);
    this->way = szHttpVersion;
    if(this->way=="GET") {
        sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
        url = szHttpVersion1;
        std::cout<<"方法是:"<<std::endl<<way<<std::endl;
        std::cout<<"请求的url是:"<<std::endl<<url<<std::endl;
    }
    else if(this->way=="POST") {
        char szHttpVersion2[128] = {0};
        char szHttpVersion3[128] = {0};
        sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
        this->url = szHttpVersion1;
        std::cout << "方法是:" << std::endl << this->way << std::endl;
        std::cout << "请求的url是:" << std::endl << this->url << std::endl;
        for (int i = 0; i < 7; i++) {
            ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
        }
        sscanf(szLineBuf, "%*[^N]%*[^a]%*[^m]%*[^e]%*[^=]=%[^&]", szHttpVersion2);
        sscanf(szLineBuf, "%*[^&]%*[^I]%*[^D]%*[^=]=%[^&]", szHttpVersion3);
        this->Name = szHttpVersion2;
        this->ID = szHttpVersion3;
        std::cout << "Name:" << std::endl << this->Name << std::endl;
        std::cout << "ID:" << std::endl << this->ID << std::endl;
        //如果POST请求的key不是Name和ID，则Name和ID的字符串为空
    }
    std::cout << "解析结束~~~"<< std::endl;
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
        std::cout << this->buf;
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