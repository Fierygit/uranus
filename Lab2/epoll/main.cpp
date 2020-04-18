#include <iostream>
#include "http_server.h"


int main() {

    auto *server = new http_server();
    server->start();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
