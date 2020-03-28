/*
 * @Author: Firefly
 * @Date: 2020-02-28 11:22:49
 * @Descripttion: 
 * @LastEditTime: 2020-02-28 11:29:23
 */
#include "testwork.h" 


void work(void) {
  int cnt = rand() % 10 + 1;
  cout << cnt << endl;
  for (int i = 0; i < cnt; i++) {
    cout << (unsigned)pthread_self() % 100 << ":\t" << i << endl;
    sleep(1);
  }
}

// 调用 suduku的解，， 用一个全局变量统计这个函数调用的时间
void solve(){


}