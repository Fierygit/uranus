//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_PARTICIPANTHANDLER_H
#define LAB3_PARTICIPANTHANDLER_H

#include "CoServer.h"
#include "../common/loguru.hpp"

void send2PaSync(std::string msg, Participants &participants, uranus::ThreadPool *threadPool);


#endif //LAB3_PARTICIPANTHANDLER_H
