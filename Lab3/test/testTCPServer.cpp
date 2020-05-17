//
// Created by Firefly on 2020/5/17.
//

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;


int main() {

    int port = 8888;
    std::string ip;
    int serverSockfd;

    struct sockaddr_in serverAddr;   //服务器网络地址结构体
    serverAddr.sin_family = PF_INET;    //设置为IP通信
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if ((serverSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "socket" << endl;
        exit(1);
    }

    //绑定地址
    if (bind(serverSockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <
        0) {
        cout << "bind" << endl;
        exit(EXIT_FAILURE);
    }

    //监听
    if (listen(serverSockfd, SOMAXCONN) < 0) {
        cout << "listen" << endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clientAddr{}; //客户端网络地址结构体, 值类型， accpet通过指针赋值
    int addrLen = sizeof(clientAddr);
    cout << "waiting for new client" << endl;
    int clientSocket;
    if ((clientSocket = accept(serverSockfd, (struct sockaddr *) &clientAddr,
                               reinterpret_cast<socklen_t *>(&addrLen))) < 0) {
        // 这里出错了不能 停止服务， 会是什么错误呢？??? //todo how to deal
        cout << "accept error" << endl;
    }
    printf("a new client is arrive : %s", inet_ntoa(clientAddr.sin_addr));



    /**
     * 测试 recv
     *
     * 测试结果， 如果是对方突然关闭， 收到0 的
     * 正常关闭收到 -1
     */


    char buf[BUFSIZ];  //数据传送的缓冲区
    while(true){
//    printf("connected to server\n");
        int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
        buf[len] = '\0';
        cout << len << endl;
        //std::cout << buf << std::endl;
    }

    return 0;
}