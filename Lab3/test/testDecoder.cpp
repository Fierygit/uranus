//
// Created by 陈杰 on 2020/5/14.
//
#include <iostream>
#include <string>
#include "../common/Util.h"
#include "../common/public.h"
void outPut(Command cmd){
    std::cout<<"OP: "<<cmd.op<<" key: "<<cmd.key<<" value: "<<cmd.value<<'\n';
}
void test(std::string str){
    outPut(Util::Decoder(str));
}
int main(){
    test("*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n");
    test("*2\r\n$3\r\nGET\r\n$7\r\nCS06142\r\n");
    test("*3\r\n$3\r\nDEL\r\n$7\r\nCS06142\r\n$5\r\nCS162\r\n");
}
