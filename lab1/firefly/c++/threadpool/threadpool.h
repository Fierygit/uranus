/*
 * @Author: Firefly
 * @Date: 2020-02-28 10:03:24
 * @Descripttion:
 * @LastEditTime: 2020-02-28 11:20:30
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <queue>

using namespace std;

//定义任务类型
typedef void (*Task)(void);

class Threadpool {
  //定义任务队列
  queue<Task> taskQueue;
  //最大任务数量
  size_t maxCount;
  //当前任务数量
  size_t curCount;
  //条件变量
  pthread_cond_t null;
  //互斥量
  pthread_mutex_t lock;

 public:
  //构造函数,确定线程池的线程数
  Threadpool(size_t cntThread, size_t maxCount);
  //析构函数
  ~Threadpool(void);
  //添加任务
  bool addTask(Task task);
  //线程入口
  static void* start(void*);
};

#endif  // THREADPOOL_H
