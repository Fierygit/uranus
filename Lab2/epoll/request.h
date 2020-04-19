//
// Created by Firefly on 2020/4/18.
//



#include <string>

// request
class httpt_request{

public:
    void response_static();
    bool is_static();

private:
    //存储你想存的东西
    std::string buf;

};



// 保存所有的连接
class clients_pool{

public:
    bool add_one(httpt_request* request);
    httpt_request* get_by_fd(int);


private:
    int num; // 连接的个数
};