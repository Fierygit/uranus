//
// Created by Firefly on 2020/5/17.
//

#include <bits/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <iostream>
#include <zconf.h>

using namespace std;


int main(){
    int clientSockfd;
    struct sockaddr_in remoteAddr; //服务器端网络地址结构体

    // remoteAddr{} 也是 0
    memset(&remoteAddr, 0, sizeof(remoteAddr)); //数据初始化--清零

    remoteAddr.sin_family = AF_INET; //设置为IP通信
    remoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
    remoteAddr.sin_port = htons(8888); //服务器端口号

    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if ((clientSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    /*将套接字绑定到服务器的网络地址上*/
    if (connect(clientSockfd, (struct sockaddr *) &remoteAddr, sizeof(struct sockaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    /**
     * 模拟发送过程中断开连接
     *
     * 如果对方是 正常关闭 返回 -1
     * 对面突然关闭连接会中断自己的连接， 要不要考虑
     */

    std::string msg;
    for(int i = 0; i < 10000000; i ++) msg.append("*");
    while(true){
        int len = send(clientSockfd, msg.c_str(), msg.size(), 0);
        cout << len << endl;
    }




    return 0;
}