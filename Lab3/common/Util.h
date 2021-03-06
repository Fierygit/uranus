//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_UTIL_H
#define LAB3_UTIL_H

#include <string>
#include "public.h"
#include <sstream>
#include <string>


class Util {
public:
    /*
     * -1 : error
     * 0 : 没有事件
     * > 0 : fd
     */
    static int recvByTime(int &fd, int time) {
        fd_set fds;
        timeval tv{};
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = time; //给 time 秒钟回复
        return select(fd + 1, &fds, nullptr, nullptr, &tv);
    }


    static Command strToCommand(std::string &buf) {
        return Command{};
    }

    static std::string outputProtocol(std::string &&str) {
        return outputProtocol(str);
    }

    static std::string outputProtocol(std::string &str) {
        std::string ret;
        for (char &c : str) {
            if (c == '\r') ret += " <-";
            else if (c == '\n') ret += "-> ";
            else ret.push_back(c);
        }
        return ret;
    }

    /*
     * //!!!解析失败请返回 null
     */
    static std::string heBin(std::string &buf) {
        std::stringstream ss;
        std::string restr;
//        int len = buf.size();
        if (*buf.begin() == '\"')buf.erase(buf.begin());
        if (*(--buf.end()) == '\"')buf.erase(--buf.end());
        int len = buf.size();
        ss << len;
        ss >> restr;
//      if(!(buf=="GET"||buf=="SET"||buf=="DEL"))
        restr = "$" + restr;
        restr += "\r\n";
        restr += buf;
        restr += "\r\n";
        return restr;
    }

    static std::string Encoder(std::string &&buf) {
        return Encoder(buf);
    }

    static std::string Encoder(std::string &buf) {
        std::stringstream ss;
        ss << buf;
        std::string catego, mid;
        std::string restr = "";
        int tot = 0;
        ss >> catego;
        if (catego == "GET" || catego == "SET" || catego == "DEL") {
            restr += heBin(catego);
            tot++;
            while (ss >> mid) {
                tot++;
                restr += heBin(mid);
            }
            if (catego == "SET") {
                if ((tot < 3) || (buf.find('\"') == buf.rfind('\"')))
                    return "null";
            }
            if (catego == "GET" && tot != 2) {
                return "null";
            }
            if (catego == "DEL" && tot < 2) {
                return "null";
            }
            ss.clear();
            ss << tot;
            std::string strint;
            ss >> strint;
            strint = "*" + strint;
            strint += "\r\n";
            restr = strint + restr;
        } else {
            restr = "null";
        }
        return restr;
    }

    static Command Decoder(std::string &&buf) {
        return Decoder(buf);
    }

    static Command Decoder(std::string &buf) {
        std::stringstream ss;
        Command reCommand;
        ss << buf;
        std::string mid;
        bool ok = false;
        while (ss >> mid) {
            if (mid == "GET") {
                reCommand.op = GET;
                ss >> mid;
                ss >> reCommand.key;
                while (ss >> mid) {
                    if (mid[0] != '$') {
                        mid = " " + mid;
                        reCommand.key += mid;
                    }
                }
                ok = true;
                break;
            } else if (mid == "SET") {
                reCommand.op = SET;
                ss >> mid;
                ss >> reCommand.key;
                ss >> mid;
                ss >> reCommand.value;
                while (ss >> mid) {
                    if (mid[0] != '$') {
                        mid = " " + mid;
                        reCommand.value += mid;
                    }
                }
                ok = true;
                break;
            } else if (mid == "DEL") {
                reCommand.op = DEL;
                ss >> mid;
                ss >> reCommand.key;
                while (ss >> mid) {
                    if (mid[0] != '$') {
                        mid = " " + mid;
                        reCommand.key += mid;
                    }
                }
                ok = true;
                break;
            }
        }
        return reCommand;
    }

    static int StrToNum(std::string str) {
        std::stringstream ss;
        int renum;
        ss << str;
        ss >> renum;
        return renum;
    }

    static bool HandleBanBao(std::string str) {
        //*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
        std::stringstream ss;
        std::stringstream strtonum;
        ss << str;
        int tot = 0;
        std::string mid;
        ss >> mid;
        if (mid[0] != '*')return false;
        else {
            mid.erase(mid.begin());
            tot = StrToNum(mid);
            int temp = 0;
            int all = 0;
            while (ss >> mid) {
                if (mid[0] != '$')return false;
                mid.erase(mid.begin());
                temp = StrToNum(mid);
                ss >> mid;
                if (temp == mid.size())all++;
                else return false;
            }
            if (tot != all)return false;
        }
        return true;
    }

    // *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    static std::string EncodeResult(std::string raw) {
        std::vector<std::string> strs;
        raw += ' ';
        std::string tmp_s;
        for (auto ch: raw) {
            if (ch == ' ') {
                strs.emplace_back(tmp_s);
                tmp_s = "";
            } else {
                tmp_s += ch;
            }
        }
        std::string res = "*" + std::to_string(strs.size()) + "\r\n";
        for (auto str: strs) {
            res += "$" + std::to_string(str.size()) + "\r\n" + str + "\r\n";
        }
        return res;
    }

private:
//    static std::stringstream ss;
};


#endif //LAB3_UTIL_H
