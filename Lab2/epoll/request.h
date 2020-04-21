//
// Created by Firefly on 2020/4/18.
//



#include <string>
#include <map>

#define BUFF_SIZE 1024

// request
class httpt_request{

public:
    void response_static();
    void get_buff(int& epfd);
    bool is_static();
    char* get_filename();
    char* get_method();
public:
    //存储你想存的东西
    std::string buf;
    bool is_sta;

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