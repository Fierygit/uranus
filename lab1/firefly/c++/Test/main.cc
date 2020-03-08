/*
 * @Author: Firefly
 * @Date: 2020-02-28 09:45:04
 * @Descripttion: 
 * @LastEditTime: 2020-02-28 10:02:57
 */

#include <iostream>

using namespace std;

int maxi(int a,int b){
    return a > b ? a : b;
}

int main(){

    // c++ 的 函数指针， 指向函数的 指针
   int (* func)(int , int) = maxi;
   cout << func(1,11) << endl;
    
}