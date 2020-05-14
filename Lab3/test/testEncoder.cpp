//
// Created by Firefly on 2020/5/12.
//

#include <iostream>
#include <string>
#include "../common/Util.h"
#include "../common/public.h"
std::string test(std::string str){
    return Util::Encoder(str);
}
int main() {
//    std::cout << "hi" << std::endl;
    std::string str;
    std::cout<<"SET Key1 Value\n";
    std::cout<<test("SET Key1 Value");
    std::cout<<"GET Key1\n";
    std::cout<<test("GET Key1");
    std::cout<<"DEL Key1 Key2\n";
    std::cout<<test("DEL Key1 Key2");
    return 0;
}