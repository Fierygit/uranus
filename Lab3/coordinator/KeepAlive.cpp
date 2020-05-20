//
// Created by Firefly on 2020/5/18.
//

#include <thread>
#include "KeepAlive.h"
#include "../common/loguru.hpp"


//使用引用， 并发的时候遍历会不会出问题？//
// 不会， 因为大小不会变化， 里面使用分段锁
void KeepAlive::init(Participants &participants) {

    for (Participant *p : participants) if (p->isAlive) p->lastAlive = std::chrono::system_clock::now();
    std::thread tmp{[&] {
        while (!Finished) {
            std::this_thread::sleep_for(checkInterval);
            keepaliveCheck(participants);
        }
    }};
    tmp.detach();
    LOG_F(INFO, "start to keepalive chaeck!!");
}

// 检测心跳在这里
// 检测到没有反应尝试连接后直接挂了p
// 1、如果是网络问题
// 2、 p故障
//      如果心跳没返回来，都重新换一条连接
//


void KeepAlive::keepaliveCheck(Participants &participants) {
    for (Participant *p : participants) {

        Time now = std::chrono::system_clock::now();
        Munites diff = std::chrono::duration_cast<Munites>(
                now - p->lastAlive);

        // 如果超时, keepalive 断掉
        if (diff.count() > invalidInterval.count()) {

            LOG_F(INFO, "out time"); // 上一次心跳时间和现在比较
        } else {

            LOG_F(INFO, "keepalive msg");
        }
    }
}
