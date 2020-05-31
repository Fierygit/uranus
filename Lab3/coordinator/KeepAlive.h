//
// Created by Firefly on 2020/5/18.
//

#ifndef LAB3_KEEPALIVE_H
#define LAB3_KEEPALIVE_H

#include <chrono>
#include <vector>
#include "../common/public.h"
#include "../common/ThreadPool.h"
#include "BoundedBlockingQueue.h"


class KeepAlive {

public:
    //using Participants = std::vector<Participant>;
    using Munites = std::chrono::minutes;

    KeepAlive(int checkInterval, int invalidInterval)
            : checkInterval{checkInterval}, invalidInterval{invalidInterval} {
    }

    ~KeepAlive() { Finished = true; }

    void init(Participants &participants, uranus::ThreadPool *poll,BoundedBlockingQueue<TaskNode> *);

private:
    void keepaliveCheck(Participants &participants, std::atomic<bool> &needSyncData, uranus::ThreadPool *poll);

    static bool connectLostPa(Participant *p);

    static void sendAndRecv(Participant *p);

private:
    int checkInterval{};
    Munites invalidInterval{}; // 判定失效间隔
    volatile bool Finished{false};
};


#endif //LAB3_KEEPALIVE_H
