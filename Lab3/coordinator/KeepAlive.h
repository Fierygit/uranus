//
// Created by Firefly on 2020/5/18.
//

#ifndef LAB3_KEEPALIVE_H
#define LAB3_KEEPALIVE_H

#include <chrono>
#include <vector>
#include "../common/public.h"


class KeepAlive {

public:
    //using Participants = std::vector<Participant>;
    using Munites = std::chrono::minutes;

    KeepAlive(int checkInterval, int invalidInterval)
            : checkInterval{checkInterval}, invalidInterval{invalidInterval} {
    }

    ~KeepAlive() { Finished = true; }

    void init(Participants& participants);

private:
    static void keepaliveCheck(Participants& participants);

private:
    Munites checkInterval;   // 心跳检测间隔
    static Munites invalidInterval; // 判定失效间隔
    volatile bool Finished{false};
};


#endif //LAB3_KEEPALIVE_H
