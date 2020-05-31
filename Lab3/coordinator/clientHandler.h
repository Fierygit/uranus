//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_CLIENTHANDLER_H
#define LAB3_CLIENTHANDLER_H

#include "../common/loguru.hpp"
#include "CoServer.h"

struct SubClientContex {
    CoServer *coServer;
    Client client;
};


void clientAcceptHandler(CoServer *);

void clientReadHandler(SubClientContex);

#endif //LAB3_CLIENTHANDLER_H
