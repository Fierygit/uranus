//
// Created by Firefly on 2020/5/17.
//

#include "PaServer.h"
#include "../common/loguru.hpp"
#include "../common/Util.h"
#include "../common/public.h"
#include <arpa/inet.h>
#include <iostream>
#include <thread>

/* NOTE:
 * 协议: 协调者返回的数据格式
 * 对于1p, 协调者返回SET STATUS "0" / "1" 代表 1phase 成功或失败
 * 对于2p, 协调者返回SET STATUS "0" 代表操作失败
 *      返回 SET ${val} XXX 代表 get的结果
 *      返回 SET STATUS "1" 代表set或del成功
 */

const int PRE_PHASE1 = 0;
const int AFTER_PHASE1 = 1;

PaServer &PaServer::init() {
    // 日志索引初始化
    latestIndex = 0;
    LOG_F(INFO, ":ip %s, port: %d", ip.c_str(), port);

    serverAddr.sin_family = PF_INET;    //设置为IP通信
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
    if ((serverSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_F(ERROR, "socket error");
        exit(1);
    }

    int ret = 0;
    int reuse = 1;
    ret = setsockopt(serverSockfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&reuse , sizeof(int));

    if (ret < 0) {
        perror("setsockopt1");
        exit(-1);
    }
    ret = setsockopt(serverSockfd, SOL_SOCKET, SO_REUSEPORT,(const void *)&reuse , sizeof(int));
    if (ret < 0) {
        perror("setsockopt2");
        exit(-1);
    }

    //绑定地址
    if (bind(serverSockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <
        0) {
        LOG_F(ERROR, "bind error");
        exit(EXIT_FAILURE);
    }

    //监听
    if (listen(serverSockfd, SOMAXCONN) < 0) {
        LOG_F(ERROR, "listen error");
        exit(EXIT_FAILURE);
    }

    LOG_F(INFO, "start to listen ...");
    return *this;
}

void PaServer::run() {
    // 创建一个新的连接，接受请求， 当有一个
    for (;;) {
        struct sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        int clientSocket;
        if ((clientSocket = accept(serverSockfd, (struct sockaddr *) &clientAddr,
                                   reinterpret_cast<socklen_t *>(&addrLen))) < 0) {
            // 这里出错了不能 停止服务， 会是什么错误呢？??? //todo how to deal？？？？？？？？？？？？？
            LOG_F(ERROR, "accept error");
            continue;
        }
        LOG_F(INFO, "a new client is arrive : %s", inet_ntoa(serverAddr.sin_addr));
        this->counter++;
        // 注意格式
        std::thread handle(&PaServer::handleCoor, this, clientSocket);
        handle.detach();
    }
    LOG_F(INFO, "should not run here!!!");
}

// 设置该参与者所属协调者, 但其实不用设置? 因为它只需要等待协调者跟它连接, 他不主动连接
void PaServer::setCoordinator(std::string ip, int port) {
    // 不需要设置
    return;
}

// 处理协调者的通信
void PaServer::handleCoor(int clientSocket) {
    char buf[BUFSIZ];  //数据传送的缓冲区
    // 0 : 普通情况, 可以执行
    // 1 : 进入了1phase, 等待1phase
    int status = 0;
    Command command_1phase{};  // 接收的命令
    for (;;) {
        LOG_F(INFO, "waiting for recv ...");
        int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
        buf[len] = '\0';
        LOG_F(INFO, "recv success .");
        LOG_F(INFO, "recv msg: %s", buf);
        // 一定要先处理
        if (len <= 0) { // 如果co 挂了
            LOG_F(WARNING, "connection closed!!!");
            this->counter--;
            break;
        }

        Command command = Util::Decoder(buf);
        LOG_F(INFO, "get command from queue OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(),
              command.value.c_str());

        // 执行命令且成功
        bool command_run = true;
        std::string send_msg;

        // 心跳包的处理
        if (command.op == GET && command.key == "${alive}") {
            LOG_F(INFO, "GET ${alive}");
            send_msg = "SET ${alive} \"${alive}\"";
            LOG_F(INFO, "latestIndex: %d", this->latestIndex);
            command_run = false;
        }
            // 添加返回索引功能
        else if (command.op == GET && command.key == "${LatestIndex}") {
            LOG_F(INFO, "CoServer: GET \"${LatestIndex}\"");
            send_msg = "SET ${LatestIndex} \"" + std::to_string(this->latestIndex) + "\"";
            command_run = false;
        }
            // 作为 leader, 将数据发送给协调者
        else if (command.op == GET && command.key == "${KVDB}") {
            LOG_F(INFO, "CoServer: request to sync data as leader");
            // 被同步数据
            send_msg = "SET ${KVDB_cnt} \"" + std::to_string(this->KVDB.size()) + "\"";
            command_run = false;
            LOG_F(INFO, "to send: %s", send_msg.c_str());
            // 统一发送
            send_msg = Util::Encoder(send_msg);
            LOG_F(INFO, "to send: %s", Util::outputProtocol(send_msg).c_str());
            if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
                LOG_F(ERROR, "part send error !");
            } else {
//                 在循环内发送数据
//                 接一个反馈发一个数据
                for (const auto &item: KVDB) {
                    LOG_F(INFO, "waiting for GET ${KVDB_next} ...");
                    int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
                    buf[len] = '\0';
                    LOG_F(INFO, "recv msg: %s", buf);
                    // 一定要先处理
                    if (len <= 0) { // 如果co 挂了
                        LOG_F(WARNING, "connection closed!!!");
                        break;
                    }

                    Command command = Util::Decoder(buf);
                    LOG_F(INFO, "get command from queue OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(),
                          command.value.c_str());

                    if (command.op == GET && command.key == "${KVDB_next}") {
                        send_msg = "SET " + item.first + " \"" + item.second + "\"";
                        LOG_F(INFO, "to send: %s", send_msg.c_str());
                        // 统一发送
                        send_msg = Util::Encoder(send_msg);
                        LOG_F(INFO, "to send: %s", Util::outputProtocol(send_msg).c_str());
                        if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
                            LOG_F(ERROR, "part send error !");
                        }
                    } else {
                        LOG_F(ERROR, "GET ${KVDB_next} receive error");
                    }
                }
            }
        }
            // 作为被同步的对象, 指明了循环的次数
        else if (command.op == SET && command.key == "${KVDB_sync_one}") {
            // 收到需要同步的消息, 先重置索引
            // TODO: 这里有个小问题: 如果重置了, 然后其他的都是新的, 就不用同步, 但是数据不一致
            // TODO: 考虑一些特殊情况
            this->latestIndex = 0;
            this->KVDB.clear();

            // 分离 latestIndex 和 loop_cnt
            std::string latestIndexStr, loopCntStr;
            bool left = true;
            for (char ch: command.value) {
                if (ch == '_') {
                    left = false;
                    continue;
                }
                if (left) {
                    latestIndexStr += ch;
                } else {
                    loopCntStr += ch;
                }
            }
            int latestIndex = atoi(latestIndexStr.c_str());
            int loopCnt = atoi(loopCntStr.c_str());
            LOG_F(INFO, "CoServer: latestIndex: %d, loopCnt: %d", latestIndex, loopCnt);

            LOG_F(INFO, "CoServer: sync it single");
            command_run = false;
            // 被同步数据
            send_msg = "SET ${KVDB_sync_one} \"OK\"";
            LOG_F(INFO, "to send: %s", send_msg.c_str());
            // 统一发送
            send_msg = Util::Encoder(send_msg);
            LOG_F(INFO, "to send: %s", Util::outputProtocol(send_msg).c_str());
            if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
                LOG_F(ERROR, "part send error !");
            } else {
                while (loopCnt--) {
                    // 接一个设置再发一个反馈
                    LOG_F(INFO, "waiting for KVDB_sync_one ...");
                    int len = recv(clientSocket, buf, BUFSIZ, 0);//接收服务器端信息
                    buf[len] = '\0';
                    LOG_F(INFO, "recv msg: %s", buf);
                    // 一定要先处理
                    if (len <= 0) { // 如果co 挂了
                        LOG_F(WARNING, "connection closed!!!");
                        break;
                    }

                    Command command = Util::Decoder(buf);
                    LOG_F(INFO, "get command from queue OP: %d\tkey: %s\tvalue: %s", command.op, command.key.c_str(),
                          command.value.c_str());

                    // 对 KV 数据库进行设置
                    if (command.op == SET) {
                        LOG_F(INFO, "KVDB: set (key:%s, val:%s)", command.key.c_str(), command.value.c_str());
                        KVDB[command.key] = command.value;

                        // 发送回馈的消息
                        send_msg = "SET SYNC_STATUS \"1\"";
                        LOG_F(INFO, "to send: %s", send_msg.c_str());
                        // 统一发送
                        send_msg = Util::Encoder(send_msg);
                        LOG_F(INFO, "to send: %s", Util::outputProtocol(send_msg).c_str());
                        if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
                            LOG_F(ERROR, "part send error !");
                        }
                    }
                }
                this->latestIndex = latestIndex;
                LOG_F(INFO, "set this->latestIndex = %d", this->latestIndex);
            }
        }
            // 其他的 正常的命令
        else {
            // 1phase之前, 可以处理
            if (status == PRE_PHASE1) {
                // 首先判断命令是否合法(如是否是 2phase命令, 如果是那么抛弃)
                if (command.op < 0 || command.op > 2) {
                    send_msg = "SET STATUS \"0\"";
                } else {
                    // 合法返回成功
                    send_msg = "SET STATUS \"1\"";
                }
                command_1phase = command;
            } else if (status == AFTER_PHASE1) {  // 如果已经进入了 1phase, 等待2phase
                // 首先判断命令是否合法
                std::string val;
                if (command.op == 1 && command.key == "${key}" && command.value == "${commit}") {
                    // 要求提交
                    if (command_1phase.op == SET) {  // SET == 1
                        KVDB[command_1phase.key] = command_1phase.value;
                        send_msg = "SET ${REP_SET} \"${OK}\"";
                    } else if (command_1phase.op == GET) {  // GET == 0
                        if (KVDB.count(command_1phase.key) == 0) {
                            // 没有这个值
                            send_msg = "SET ${REP_GET} \"${NULL}\"";
                        } else {
                            val = KVDB[command_1phase.key];
                            send_msg = "SET ${REP_GET} " + std::string("\"" + val + "\"");
                        }
                    } else if (command_1phase.op == DEL) {  // DEL == 2
                        // TODO: 考虑 del 为空以及 del 多个key的情况. 多个 del key 用空格隔开了
                        LOG_F(INFO, "%s", command_1phase.key.c_str());
                        std::vector<std::string> del_keys;
                        command_1phase.key += ' ';
                        std::string tmp_s;
                        for (auto ch: command_1phase.key) {
                            if (ch == ' ') {
                                del_keys.emplace_back(tmp_s);
                                tmp_s = "";
                            } else {
                                tmp_s += ch;
                            }
                        }
                        int del_ok_cnt  = 0;
                        for (auto del_key: del_keys) {
                            if (KVDB.count(del_key)) {
                                del_ok_cnt += 1;
                                KVDB.erase(del_key);
                                LOG_F(INFO, "del %s", del_key.c_str());
                            }
                        }
                        send_msg = "SET DEL_CNT \"" + std::to_string(del_ok_cnt) + "\"";
                        LOG_F(INFO, "del msg: %s", send_msg.c_str());
                    } else {  // 命令不合法
                        send_msg = "SET STATUS \"0\"";
                    }
                } else {
                    // 要求终止
                    send_msg = "SET STATUS \"0\"";
                }
            }
        }


        LOG_F(INFO, "to send: %s", send_msg.c_str());
        // 统一发送
        send_msg = Util::Encoder(send_msg);
        LOG_F(INFO, "to send: %s", Util::outputProtocol(send_msg).c_str());
        if (send(clientSocket, send_msg.c_str(), send_msg.size(), 0) != send_msg.size()) {
            LOG_F(ERROR, "part send error !");
        } else {
            // 如果是执行普通的命令, 需要跳转状态
            if (command_run) {
                if (status == PRE_PHASE1) {
                    status = AFTER_PHASE1;
                    LOG_F(INFO, "status: PRE_PHASE1 -> AFTER_PHASE1");
                } else {
                    status = PRE_PHASE1;
                    // TODO: 需要添加 get 操作不影响索引
                    this->latestIndex += 1;
                    LOG_F(INFO, "NOTE: this->latestIndex += 1, latestIndex: %d", this->latestIndex);
                    LOG_F(INFO, "status: AFTER_PHASE1 -> PRE_PHASE1");
                }
            }
        }
    }
}

