//
// Created by Firefly on 2020/5/20.
//

#include <sys/socket.h>
#include <thread>
#include "syncDataHandler.h"
#include "../common/Util.h"

int getAliveCnt(Participants& participants) {
    int wait_cnt = 0;
    for (const auto &p: participants) {
        if (p->isAlive) {
            wait_cnt++;
        }
    }
    return wait_cnt;
}

// 同步数据库
void syncKVDB(Participants & participants) {
    WaitGroup waitSyncGroup;
    LOG_F(INFO, "this->getAliveCnt(): %d", getAliveCnt(participants));
    //todo 中途有新的 p 加入进来怎么办？？？？？？？？？？？？？？？？
    waitSyncGroup.Add(getAliveCnt(participants));
    std::vector<int> result(participants.size());
    int idx = -1;
    for (Participant *p: participants) {
        idx++;
        if (!p->isAlive) continue;
        // 注意用法
        std::thread handleSync([p, &waitSyncGroup, &idx, &result] {
            getLatestIndex(p, idx, result);
            waitSyncGroup.Done();
        });
        handleSync.detach();
    }
    waitSyncGroup.Wait();

    // 打印 LOG信息, 获取最大索引
    int maxLogIndex = 0, maxIndex = -1;
    for (int i = 0; i < participants.size(); i++) {
        if (!participants[i]->isAlive) continue;
        LOG_F(INFO, "alive p(%s:%d) latestIndex: %d", participants[i]->ip.c_str(), participants[i]->port,
              participants[i]->lastIndex);
        if (maxLogIndex < participants[i]->lastIndex) {
            maxLogIndex = participants[i]->lastIndex;
            maxIndex = i;
        }
    }
    // 获取leader
    if (maxIndex != -1) {
        Participant *mainPart = participants[maxIndex];
        LOG_F(INFO, "maxLogIndex: %d, max part: (%s:%d)", maxLogIndex, mainPart->ip.c_str(), mainPart->port);
        // 开始查找需要进行同步的参与者
        std::vector<Participant *> toSyncParts;
        for (auto & participant : participants) {
            if (!participant->isAlive) continue;
            if (maxLogIndex > participant->lastIndex) {
                toSyncParts.emplace_back(participant);
                LOG_F(INFO, "should sync: p(%s:%d) latestIndex: %d", participant->ip.c_str(), participant->port,
                      participant->lastIndex);
            }
        }
        if (toSyncParts.empty()) {
            LOG_F(INFO, "nothing to sync ...");
        } else {
            LOG_F(INFO, "%s", (std::to_string(toSyncParts.size()) + std::string(" ps waiting to sync")).c_str());
            // 首先获得 leader(假设) 的数据库信息
            std::vector<std::string> leaderData = getLeaderData(mainPart);
            for (auto s: leaderData) {
                LOG_F(INFO, "leaderData: %s", Util::outputProtocol(s).c_str());
            }

            // 然后使用多线程将它同步给每个缺失信息的数据库
            for (auto &p: toSyncParts) {
                std::thread handleOneSync([p, leaderData, maxLogIndex] {
                    LOG_F(INFO, "start: sync for (%s:%d)", p->ip.c_str(), p->port);
                    syncOnePart(p, leaderData, maxLogIndex);
                });
                handleOneSync.detach();
            }
        }
    } else {
        LOG_F(INFO, "nothing to sync ...");
    }
}



// 获取一个 p 最新的 操作(日志)索引
void getLatestIndex(Participant *p, int idx, std::vector<int> &result) {
    LOG_F(INFO, "Trying to get Lastest index");

    // 协议
    std::string msg = Util::Encoder("GET \"${LatestIndex}\"");

    std::unique_lock<std::mutex> uniqueLock(p->lock); // 获取锁
    if (send(p->fd, msg.c_str(), msg.size(), 0) != msg.size()) {
        LOG_F(WARNING, "participant %d send error!!!", p->port);
    } else {            //发送完等待接受
        LOG_F(INFO, "participant %d send \"GET \"${LatestIndex}\" success. waiting for receive...", p->port);
        char buf[BUFSIZ];  //数据传送的缓冲区
        int len = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
        buf[len] = '\0';
        if (len <= 0) { // 如果co 挂了
            LOG_F(WARNING, "participant %d connection closed!!!", p->port);
        }
        LOG_F(INFO, "receive: len: %d  %s", len, Util::outputProtocol(buf).c_str());
        Command command = Util::Decoder(buf);

        if (command.op == SET && command.key == "${LatestIndex}") {
            LOG_F(INFO, "receive %d OP: %d\tkey: %s\tvalue: %s", p->port,
                  command.op, command.key.c_str(), command.value.c_str());
            // 保存结果
            p->lastIndex = atoi(command.value.c_str());// NOLINT(cert-err34-c)
            LOG_F(INFO, "set %s:%d latestIndex: %d", p->ip.c_str(), p->port, p->lastIndex);
            result[idx] = atoi(command.value.c_str()); // NOLINT(cert-err34-c)
        }
    }
}

/* 协议: 获取leader的全部数据, 用来同步那些落后的参与者
 * C to P: GET "${KVDB}"
 * P to C: SET ${KVDB_cnt} "${KVDB.size()}"
 */
std::vector<std::string> getLeaderData(Participant *p) {
    std::vector<std::string> leaderData;
    // 协议, 获取整个KVDB
    std::string rmsg = Util::Encoder("GET \"${KVDB}\"");

    std::unique_lock<std::mutex> uniqueLock(p->lock); // 获取锁
    if (send(p->fd, rmsg.c_str(), rmsg.size(), 0) != rmsg.size()) {
        LOG_F(WARNING, "participant %d send error!!!", p->port);
    } else {            //发送完等待接受
        LOG_F(INFO, "participant %d send \"GET \"${KVDB}\" success. waiting for receive...", p->port);
        char buf[BUFSIZ];  //数据传送的缓冲区
        // 先接受 长度, 再根据长度接收所有的数据
        int KVDB_cnt = 0;

        int len = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
        buf[len] = '\0';
        if (len <= 0) { // 如果co 挂了
            LOG_F(WARNING, "participant %d connection closed!!!", p->port);
        }
        LOG_F(INFO, "receive: len: %d  %s", len, Util::outputProtocol(buf).c_str());
        Command command = Util::Decoder(buf);
        LOG_F(INFO, "receive %d OP: %d\tkey: %s\tvalue: %s", p->port,
              command.op, command.key.c_str(), command.value.c_str());

        // 数量
        if (command.op == SET && command.key == "${KVDB_cnt}") {
            // 保存结果
            KVDB_cnt = atoi(command.value.c_str());// NOLINT(cert-err34-c)
            LOG_F(INFO, "%s", ("KVDB_cnt : " + std::to_string(KVDB_cnt)).c_str());

            while (KVDB_cnt--) {
                // 获取下一个
                std::string msg = Util::Encoder("GET \"${KVDB_next}\"");
                LOG_F(INFO, "to send: %s", Util::outputProtocol(msg).c_str());
                if (send(p->fd, msg.c_str(), msg.size(), 0) != msg.size()) {
                    LOG_F(ERROR, "send error: GET \"${KVDB_next}\"");
                } else {
                    int len1 = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
                    buf[len1] = '\0';
                    if (len1 <= 0) { // 如果co 挂了
                        LOG_F(WARNING, "participant %d connection closed!!!", p->port);
                    }
                    LOG_F(INFO, "receive: len: %d  %s", len1, Util::outputProtocol(buf).c_str());
                    Command command1 = Util::Decoder(buf);
                    LOG_F(INFO, "receive %d OP: %d\tkey: %s\tvalue: %s", p->port,
                          command1.op, command1.key.c_str(), command1.value.c_str());

                    leaderData.emplace_back(buf);
                }
            }
        }
    }
    return leaderData;
}



/* 同步协议: 同步单个参与者, 使用leaderData
 * CoServer to PaServer: SET ${KVDB_sync_one} "maxIndex_syncSize"
 * PaServer to CoServer: SET ${KVDB_sync_one} "OK"
 * loop: size = syncSize
 *      CoServer to PaServer: SET ${key} "${value}"
 *      PaServer to CoServer: SET SYNC_STATUS "1"
 *
 *  例子:
 *  C to P: SET ${KVDB_sync_one} "5_3", 5代表当前日志的索引, 3代表数据库有3个数据需要更新
 *  P to C: SET ${KVDB_sync_one} "OK", 代表正确接收 KVDB_sync_one 的请求
 *  然后循环3次, 因为 syncSize = 3:
 *      C to P: SET a "value", 将 a 设置成 "value" 值
 *      SET SYNC_STATUS "1", 表示同步成功
 */
void syncOnePart(Participant *p, const std::vector<std::string>& leaderData, int maxIndex) {
    std::unique_lock<std::mutex> uniqueLock(p->lock);// 获取锁
    std::vector<std::string> newLeaderData = leaderData;
    LOG_F(INFO, "(%s:%d) doing syncOnePart ...", p->ip.c_str(), p->port);

    // 加入了索引信息
    std::string tmpMsg = Util::Encoder(
            "SET ${KVDB_sync_one} \"" + std::to_string(maxIndex) + "_" + std::to_string(newLeaderData.size()) + "\"");
//    std::cout << "SET ${KVDB_sync_one} \"" + std::to_string(maxIndex) + "_" + std::to_string(newLeaderData.size()) + "\"" << std::endl;
//    LOG_F(INFO, "syncOnePart: to send: %s", msg.c_str());
    newLeaderData.insert(newLeaderData.begin(), tmpMsg);
    char buf[BUFSIZ];  //数据传送的缓冲区

    for (const auto &msg: newLeaderData) {
        LOG_F(INFO, "syncOnePart: to send: %s", msg.c_str());
        if (send(p->fd, msg.c_str(), msg.size(), 0) != msg.size()) {
            LOG_F(ERROR, "send error: GET \"${KVDB_next}\"");
        } else {
            int len = recv(p->fd, buf, BUFSIZ, 0);//接收服务器端信息
            buf[len] = '\0';
            if (len <= 0) { // 如果co 挂了
                LOG_F(WARNING, "participant %d connection closed!!!", p->port);
            }
            LOG_F(INFO, "receive: len: %d  %s", len, Util::outputProtocol(buf).c_str());
            Command command = Util::Decoder(buf);
            LOG_F(INFO, "receive %d OP: %d\tkey: %s\tvalue: %s", p->port,
                  command.op, command.key.c_str(), command.value.c_str());
        }
    }

    LOG_F(INFO, "end: sync for (%s:%d)", p->ip.c_str(), p->port);
}