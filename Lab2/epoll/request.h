//
// Created by Firefly on 2020/4/18.
//



#include "string.h"
#include <map>
#include <sstream>
#include <iostream>

#define BUFF_SIZE 1024

// request
class httpt_request{

public:
    void response_static();
    void get_buff(int& epfd);
    bool is_static();

public:
    //存储你想存的东西
    std::string buf;
    bool is_sta;
    std::string url;
    std::string way;
    //way为http请求的方法，可能是GET、POST，或者是其他不需要实现的方法（比如DELETE or PUT, HEAD, etc）
    std::string Name;
    std::string ID;
    //如果POST请求的key不是Name和ID，则Name和ID的字符串为空
public:
    int fd;
public:
    httpt_request(int fd): fd(fd) {}
    httpt_request() {}
};



// 保存所有的连接
class clients_pool{

public:
    bool add_one(httpt_request* request);
    httpt_request* get_by_fd(int);
    std::map<int, httpt_request*> fd2request;


private:
    int num; // 连接的个数
};