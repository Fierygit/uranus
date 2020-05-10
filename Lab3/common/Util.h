//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_UTIL_H
#define LAB3_UTIL_H

#include <string>
#include "public.h"



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
        return Command();
    }

private:


};


#endif //LAB3_UTIL_H
