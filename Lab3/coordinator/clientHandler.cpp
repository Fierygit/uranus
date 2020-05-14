//
// Created by Firefly on 2020/5/10.
//

#include <thread>
#include "clientHandler.h"
#include "../common/Util.h"

#include <arpa/inet.h>

/*
 * 这是一个总的连接线程， 当收到client 的连接的时候创建一个新的线程去处理 io
 */
void clientAcceptHandler(CoServer *coServer) {

    int serverSockfd = coServer->getServerSockfd();

    // 接受新的请求
    for (;;) {
        int clientSocket;
        int addrLen;

        struct sockaddr_in clientAddr{}; //客户端网络地址结构体, 值类型， accpet通过指针赋值
        addrLen = sizeof(clientAddr);
        LOG_F(INFO, "waiting for new client");
        if ((clientSocket = accept(serverSockfd, (struct sockaddr *) &clientAddr,
                                   reinterpret_cast<socklen_t *>(&addrLen))) < 0) {
            // 这里出错了不能 停止服务， 会是什么错误呢？??? //todo how to deal
            LOG_F(ERROR, "accept error");
            continue;
        }
        LOG_F(INFO, "a new client is arrive : %s", inet_ntoa(clientAddr.sin_addr));

        /*
         * 创建一个子的线程全程处理， 新的 client 的 io，读完之后， 放到 blockingqueue
         */
        Client client{clientSocket, clientAddr, true, ""};
//        coServer->getClients()
        std::thread tmpThread{[coServer, client]() {
            clientReadHandler(SubClientContex{coServer, client});
        }};
        tmpThread.detach();
    }
    LOG_F(ERROR, "should not be run here");

}


void clientReadHandler(const SubClientContex& ctx) {
    int clientSocket = ctx.client.fd;
    sockaddr_in clientAddr = ctx.client.addr;
    std::string clientBuf;
    int len = send(clientSocket, "Welcome to my server\n", 21, 0);//发送欢迎信息

    if (len < 0) {
        LOG_F(ERROR, "client is bad : %s", inet_ntoa(clientAddr.sin_addr));
        return;
    }

    char buf[BUFSIZ];  //数据传送的缓冲区8192

    /*接收客户端的数据并将其发送给客户端--recv返回接收到的字节数，send返回发送的字节数*/
    while ((len = recv(clientSocket, buf, BUFSIZ, 0)) > 0) {
        buf[len] = '\0';
        LOG_F(INFO, "\n%s\n", buf);
        clientBuf.append(buf);
        Command command = Util::Decoder(clientBuf);

        LOG_F(INFO, "OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(), command.value.c_str());

        /*
         * 放入 blockingqueue, 让 2pc 线程处理
         */

        if (send(clientSocket, buf, len, 0) < 0) {
            LOG_F(ERROR, "client is bad : %s", inet_ntoa(clientAddr.sin_addr));
            return;
        }
        clientBuf.clear();
    }


}