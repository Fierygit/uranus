/*
 * @Author: Firefly
 * @Date: 2020-02-28 10:09:13
 * @Descripttion:
 * @LastEditTime: 2020-02-28 11:41:32
 */

#include "threadpool.h"
#include "testwork.h"

int main() {
  int task_num = 5;
  Threadpool* tpool = new Threadpool(3, task_num);
  
  //这里只添加 固定的任务数， 可以一直添加
  for (int i = 0; i < task_num; i++) tpool->addTask(work);

  pause();
}
