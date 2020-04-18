#include <iostream>
#include "http_server.h"


int main() {
    http_server server;
    server.start();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
