/*
 * @Author: Firefly
 * @Date: 2020-02-28 10:03:24
 * @Descripttion:
 * @LastEditTime: 2020-03-08 17:15:49
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <queue>

using namespace std;

//定义任务类型
typedef void (*Task)(int);

class Threadpool {
 public:
  //定义任务队列
  Task task;  // 定义所有线程的任务
  //最大任务数量
  size_t maxCount;
  //当前任务数量
  size_t curCount;
  //条件变量
  pthread_cond_t cond;
  //互斥量
  pthread_mutex_t lock;

  //构造函数,确定线程池的线程数
  Threadpool(size_t cntThread, size_t maxCount, Task task);
  //析构函数
  ~Threadpool(void);

  //线程入口
  static void* start(void*);
  
};

#endif  // THREADPOOL_H
