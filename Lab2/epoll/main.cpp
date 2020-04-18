#include <iostream>
#include "http_server.h"


int main() {
    http_server* server = new http_server(7777);
    server->start();
    std::cout << "shutdown!\n";
//    http_server server;
//    server.start();
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
}
