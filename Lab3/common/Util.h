//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_UTIL_H
#define LAB3_UTIL_H

#include <string>
#include "public.h"
#include <sstream>



class Util {
public:
    /*
     * 字符串到 Command，解析不到返回空的
     */

    static Command strToCommand(std::string &buf) {
        return Command{};
    }

    /*
     * //!!!解析失败请返回 null
     */
    static std::string heBin(std::string &buf){
        std::stringstream ss;
        std::string restr;
//        int len = buf.size();
        if(*buf.begin()=='\"')buf.erase(buf.begin());
        if(*(--buf.end())=='\"')buf.erase(--buf.end());
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
    static std::string Encoder(std::string &buf) {
        std::stringstream ss;
        ss<<buf;
        std::string catego,mid;
        std::string restr="";
        int tot = 0;
        ss>>catego;
        if(catego=="GET"||catego=="SET"||catego=="DEL"){
            restr += heBin(catego);
            tot++;
            //*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
            while(ss>>mid){
                tot++;
                restr += heBin(mid);
            }
            if(catego=="SET"&&tot<3){
                return "null";
            }
            if(catego=="GET"&&tot!=2){
                return "null";
            }
            if(catego=="DEL"&&tot<2){
                return "null";
            }
            ss.clear();
            ss << tot;
            std::string strint;
            ss >> strint;
            strint = "*" + strint;
            strint += "\r\n";
            restr = strint + restr;
        }else{
            restr = "null";
        }
        return restr;
    }

    static Command Decoder(std::string &buf) {
        std::stringstream ss;
        Command reCommand;
        ss<<buf;
        std::string mid;
        bool ok = false;
        while(ss>>mid){
            if(mid=="GET"){
                reCommand.op = GET;
                ss>>mid;
                ss>>reCommand.key;
                while(ss>>mid){
                    if(mid[0]!='$'){
                        mid = " " + mid;
                        reCommand.key += mid;
                    }
                }
                ok = true;
                break;
            }else if(mid=="SET"){
                reCommand.op = SET;
                ss>>mid;
                ss>>reCommand.key;
                ss>>mid;
                ss>>reCommand.value;
                while(ss>>mid){
                    if(mid[0]!='$'){
                        mid = " " + mid;
                        reCommand.value += mid;
                    }
                }
                ok = true;
                break;
            }else if(mid=="DEL"){
                reCommand.op = DEL;
                ss>>mid;
                ss>>reCommand.key;
                while(ss>>mid){
                    if(mid[0]!='$'){
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

private:
//    static std::stringstream ss;
};


#endif //LAB3_UTIL_H
