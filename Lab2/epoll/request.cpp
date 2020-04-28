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
#include <stdio.h>
#include <cstring>

bool clients_pool::add_one(httpt_request *request) {
    this->fd2request[request->fd] = request;
    return false;
}

httpt_request *clients_pool::get_by_fd(int client_sockt) {
    return fd2request[client_sockt];
}

bool httpt_request::is_static() {
    DEBUG_LOG("HTTP方法与url获取~~~");
    this->httpValid = false;
    while(!this->buf.empty()) {
        std::string endValid="\r\n\r\n";
        std::string::size_type idx;
        idx=this->buf.find(endValid);
        if(idx == std::string::npos) {
            httpValid=false;
            return this->is_sta;
        }
        else httpValid=true;
        char szLineBuf[128] = {0};
// 通过流来提取报文头中内容
        int sizeBuf= sizeof(this->buf);
        std::stringstream ssHeader(buf.c_str());
// 提取第一行内容
        ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
        char  szHttpVersion[128] = {0};
        char szHttpVersion1[128] = {0};
        sscanf(szLineBuf,  "%[^ ]", szHttpVersion);
        this->way = szHttpVersion;

        if(this->way=="POST") {
            char szHttpVersion2[128] = {0};
            char szHttpVersion3[128] = {0};
            sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
            this->url = szHttpVersion1;
            DEBUG_LOG("方法是:%s",this->way.c_str());
            DEBUG_LOG("请求的url是:%s",this->url.c_str());
            int point=0;
            int Keylength=0;
            point=this->buf.find("Content-Length:");
//        cout<<endl<<point<<endl;
            for(int i=point+16; buf[i]!='\r'; i++) {
                Keylength=Keylength*10;
                Keylength+=(int)buf[i]-48;
            }
//        cout<<Keylength<<endl;
            point=buf.find('&');
            for(int i=point; buf[i]!='\n'; i--) {
                point--;
            }
//         cout<<point<<endl;
            std::string KeyValid;
            for(int i=point+1; i<point+1+Keylength; i++) {
                KeyValid.push_back(buf[i]);
            }
//         cout<<KeyValid<<endl;
            bool KEY=false;
            if(KeyValid[0]=='N'&&KeyValid[1]=='a'&&KeyValid[2]=='m'&&KeyValid[3]=='e'&&KeyValid.find("ID")) {
                KEY=true;
            }

            if(KEY) {
                for(int i=5; KeyValid[i]!='&'; i++) {
                    this->Name.push_back(KeyValid[i]);
                }
                for(int i=5,j=0; i<Keylength; i++) {
                    if(j>=1) {
                        this->ID.push_back(KeyValid[i]);
                    }
                    if(KeyValid[i]=='=')j++;
                }
                DEBUG_LOG("Name:%s ID:%s",this->Name.c_str(),this->ID.c_str());
//                cout<<"Name:"<<Name<<"  ID:"<<ID<<endl;
            }

            std::string::iterator it;
            it=this->buf.begin();
            std::string::iterator it_1 = this->buf.begin();
            buf.erase(it, it+point+Keylength+1);
        }
        else {
            sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
            this->url = szHttpVersion1;
            DEBUG_LOG("方法是:%s",this->way.c_str());
            DEBUG_LOG("请求的url是:%s",this->url.c_str());
            std::string::iterator it;
            it=this->buf.begin();
            int piontget=buf.find("\r\n\r\n");
            std::string::iterator it_1 = this->buf.begin();
            this->buf.erase(it, it+piontget+4);
        }
//到此解析完一个完整的http协议，如果是两个协议连着一起发过来，则循环到开始解析下一个
    }
    DEBUG_LOG("解析结束~~~");
    return this->is_sta;
}

int httpt_request::getlen (const char*file){
    // printf("fffffffffffffffffffff: %s\n",file);
    FILE * pFile;
    int size=0;
    pFile = fopen (file,"rb");
    if (pFile==NULL)
        perror ("Error opening file");
    else{
        fseek (pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
        size=ftell (pFile); ///求出当前文件指针距离文件开始的字节数
        fclose (pFile);
        // printf ("Size of file.cpp: %ld bytes.\n",size);
    }
    return size;
}

void httpt_request::response_static() {
    memset(response,0,sizeof(response));
    //获得文件名，以及方法
    const char *filename=get_filename();
    const char *method=way.c_str();
    char*mime;
    if(strstr(filename,".html"))
        mime=(char*)"text/html";
    else if(strstr(filename,".jpg")){
        mime=(char*)"image/jpeg";
    }else if(strstr(filename,".ico")){
        mime=(char*)"image/jpeg";
    }
    //webbench -c 10 http://127.0.0.1:8888/index.html
    if(this->way=="GET"){//GET方法
        //确定mime类型
        //获得文件描述符
        int filefd=open(filename,O_RDONLY);
        // printf("mime: %s\n",mime);
        sprintf(response,"HTTP/1.1 ");
        if(filefd<0){//不存在该文件
            sprintf(response+strlen(response),"404 NOT FOUND\r\nContent-Type: %s\r\n\r\n",mime);
            filefd=open("404.html",O_RDONLY);
        }else{
            //得到文件的长度
            int len=getlen(filename);
            //Content-Length
            sprintf(response+strlen(response),"200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",mime,len);
        }
        write(fd,response,strlen(response));
        memset(response,0,sizeof(response));
        while(1){
            int filelen=read(filefd,response,1);
            if(filelen<=0)break;
            write(fd,response,filelen);
        }
        close(filefd);
    }else if(this->way==(char*)"POST"){
        // printf("POST METHOD--------------------------\n");
        if(this->Name=="Name"&&this->ID=="ID"){
            sprintf(response,"HTTP/1.1 200 OK\r\n\r\n");
            write(fd,response,strlen(response));
        }else{
            sprintf(response,"HTTP/1.1 ");
            sprintf(response+strlen(response),"404 NOT FOUND\r\nContent-Type: %s\r\n\r\n",mime);
            int filefd=open("404.html",O_RDONLY);
            write(fd,response,strlen(response));
            memset(response,0,sizeof(response));
            while(1){
                int filelen=read(filefd,response,1);
                if(filelen<=0)break;
                write(fd,response,filelen);
            }
            close(filefd);
        }
    }else{
        sprintf(response,"HTTP/1.1 501 ERROR\r\n\r\n");
        write(fd,response,strlen(response));
    }
    
    // std::cout<<"here\n";
}
const char* httpt_request::get_filename(){
    if(this->url=="/"){
        newurl="./index.html";
    }else{
        newurl="."+url;
    }
    return newurl.c_str();
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
