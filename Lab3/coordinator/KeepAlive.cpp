//
// Created by Firefly on 2020/5/18.
//

#include <thread>
#include "KeepAlive.h"
#include "CoServer.h"
#include "../common/loguru.hpp"


//使用引用， 并发的时候遍历会不会出问题？ ！！！不会， 因为大小不会变化， 里面使用分段锁
void KeepAlive::init(Participants& participants) {
    std::thread tmp{[&]{
        while (!Finished) {
            std::this_thread::sleep_for(checkInterval);
            keepaliveCheck(participants);
        }
    }};
    tmp.detach();
    LOG_F(INFO,"start to keepalive chaeck!!");
}

// 检测心跳在这里
// 检测到没有反应尝试连接后直接挂了p
// 1、 如果只是网络问题， 导致提交失败， 这里就要断了连接， 下一次再来尝试
// 2、 这里不处理重新上线， 上线了，p发消息过来？ 还是由 这里检查？？？？
void KeepAlive::keepaliveCheck(Participants& participants) {

}
