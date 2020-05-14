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
    static std::string Encoder(std::string &buf) {
        return buf;
    }

    static Command Decoder(std::string &buf) {
        Command reCommand;
        std::stringstream ss;
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
                        reCommand.key.append(mid);
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
                        reCommand.value.append(mid);
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
                        reCommand.key.append(mid);
                    }
                }
                ok = true;
                break;
            }
        }
        return reCommand;
//        return Command();
    }

private:


};


#endif //LAB3_UTIL_H
