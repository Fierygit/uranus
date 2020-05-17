//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_PUBLIC_H
#define LAB3_PUBLIC_H

enum OP {
    GET, SET, DEL
};

//LOG_F(INFO, "%d\t%s\t%s", command.op, command.key.c_str(), command.value.c_str());

struct Command {
    OP op;
    std::string key;
    std::string value;
};




#endif //LAB3_PUBLIC_H
