//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_CLIENT_H
#define LAB3_CLIENT_H

#include <netinet/in.h>

class Client {

public:
    Client() {}

    Client* init();

    void run();
private:

    void sendToServer(std::string);


private:
    int clientSockfd;
    struct sockaddr_in remoteAddr; //服务器端网络地址结构体
};


#endif //LAB3_CLIENT_H
