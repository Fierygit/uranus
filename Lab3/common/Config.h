#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>

// 配置文件, 解析, 传递配置
class Config {
public:
    int isCoordinator;
    // pair 代表 <ip, port>
    // self 代表自己(根据 isCoor)
    std::pair<std::string, std::string> self;
    std::vector<std::pair<std::string, std::string>> others;

    // 解析配置文件
    void parse(const std::string &file_path);

    // 解析命令行参数
    static void parse_arg(int argc, char *argv[], std::string &file_path);

    //get banner
    static std::string getBanner(const std::string &path);

    // 分割字符
    static std::vector<std::string> split(const std::string &str, const std::string &delim);

};

// 参数解析
void Config::parse_arg(int argc, char *argv[], std::string &file_path) {
    // 长短参数解析
    const char *optstring = "c:";
    int c;
    struct option opts[] = {
            {"config_path", 1, nullptr, 'c'}};
    while ((c = getopt_long(argc, argv, optstring, opts, nullptr)) != -1) {
        switch (c) {
            case 'c':
                file_path = optarg;
                LOG_F(INFO, "arg is %s", optarg);
                break;
            case '?':
                LOG_F(ERROR, "unknown option");
                break;
            default:
                LOG_F(ERROR, "------\n");
        }
    }
}

// 解析配置文件
void Config::parse(const std::string &file_path) {
    std::ifstream ifile;
    ifile.open(file_path, std::ios::in);

    if (!ifile.is_open()) {
        LOG_F(ERROR, "config can not open!!!");
        exit(1);
    }

    std::string line;
    while (getline(ifile, line)) {
        if (line.size() == 0 || line[0] == '!') continue;
        LOG_F(INFO, "%s", line.c_str());
        std::vector<std::string> res = Config::split(line, " ");
        if (res.size() != 2) {
            continue;
        }
        if (res[0] == "mode") {
            std::string tmp;
            for (int i = 0; i < 11; i++) {
                tmp += res[1][i];
            }
            res[1] = tmp;
            if (res[1] == "coordinator") {
                this->isCoordinator = true;
                LOG_F(INFO, "parse: mode is coordinator");
            } else if (res[1] == "participant") {
                LOG_F(INFO, "parse: mode is participant");
                this->isCoordinator = false;
            }
        } else {
            std::vector<std::string> ip_port = Config::split(res[1], ":");
            if ((this->isCoordinator && res[0] == "coordinator_info") ||
                (!this->isCoordinator && res[0] == "participant_info")) {
                self = make_pair(ip_port[0], ip_port[1]);
            } else if ((!this->isCoordinator && res[0] == "coordinator_info") ||
                       (this->isCoordinator && res[0] == "participant_info")) {
                // 去除后面的换行符
                std::string tmp;
                for (auto ch: ip_port[1]) {
                    if (ch >= '0' && ch <= '9') {
                        tmp += ch;
                    } else {
                        break;
                    }
                }
                ip_port[1] = tmp;
                others.push_back(make_pair(ip_port[0], ip_port[1]));
            }
        }
    }

    ifile.close();
}

// 以 delim 分割 str
std::vector<std::string> Config::split(const std::string &str, const std::string &delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

std::string Config::getBanner(const std::string &path) {
    std::ifstream ifile;
    ifile.open(path, std::ios::in);

    if (!ifile.is_open()) {
        LOG_F(ERROR, "can not find banner");
    }
    std::string line;
    std::string ret;
    while (getline(ifile, line)) {
        ret += (line + "\n");
    }
    return ret;
}
