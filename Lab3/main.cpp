#include <iostream>
#include <string>

#include "./coordinator/CoServer.h"
#include "./participant/PaServer.h"
#include "./common/loguru.hpp"
#include "./common/Config.h"  // 读取配置文件, 并做参数解析


int main(int argc, char *argv[]) {
    std::string file_path;
    Config conf;

    // 命令行参数解析
    conf.parse_arg(argc, argv, file_path);
    LOG_F(INFO, "file path%s", file_path.c_str());

    // 解析配置文件
    conf.parse(file_path);

    if (conf.isCoordinator) {
        LOG_F(INFO, "RUNNING Coordinator");
        // 协调者
        CoServer *coServer = new CoServer(conf.self.first, atoi(conf.self.second.c_str()));
        // 设置参与者
        coServer->setParticipant(conf.others);
        // 开始运行
        coServer->init().run();
    } else {
        LOG_F(INFO, "RUNNING Participant");
        // 参与者
        PaServer *paServer = new PaServer(conf.self.first, atoi(conf.self.second.c_str()));
        LOG_F(INFO, std::to_string(conf.others.size()).c_str());
        // 设置它的协调者
        paServer->setCoordinator(conf.others[0].first, atoi(conf.others[0].second.c_str()));
        // 运行参与者
        paServer->init().run();
    }
    return 0;
}