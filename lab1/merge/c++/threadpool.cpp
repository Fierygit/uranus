/*
 * @Author: Firefly
 * @Date: 2020-02-28 10:05:34
 * @Descripttion:
 * @LastEditTime: 2020-03-08 17:19:17
 */
#include "threadpool.h"
#include <sys/time.h>

int64_t now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}
static int64_t start_t, end_t;

//构造函数,确定线程池的线程数
Threadpool::Threadpool(size_t cntThread, size_t maxCount, Task task) {
  //初始化条件变量与互斥量
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);

  this->maxCount = maxCount;

  //创建线程
  cout << "main pthread pid is " << ((unsigned)pthread_self() % 100) << endl;
  pthread_t pid[cntThread];

  pthread_mutex_lock(&lock);  // 确保所有的线程都被创建
  for (int i = 0; i < cntThread; i++) {
    // 第一个  就是 控制这个线程 句柄上面的数组
    // 第二个 是设置新的线程的，栈大小等等信息，默认为空就可以了
    // 第3个参数是一个函数指针，它返回void * 参数也是void *, 回调函数
    // 第四个 参数是线程的参数
    int ret = pthread_create(pid + i, NULL, start, this);
    while (ret != 0) {
      cout << "create pthread error! " << ret << "\n";
      int ret = pthread_create(pid + i, NULL, start, this);
    }
  }

  curCount = 0;
  this->task = task;
  cout << "start deal !!!" << endl;
  start_t = now();
  pthread_mutex_unlock(&lock);
  // 释放锁，其它线程就能拿到锁了， 开始执行！！！
}

//析构函数
Threadpool::~Threadpool(void) {
  //销毁条件变量与互斥量
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
}

//线程入口
void* Threadpool::start(void* arg) {
  Threadpool* thread = (Threadpool*)arg;
  cout << "create thread " << ((unsigned)pthread_self() % 100) << endl;

  while (true) {
    //加锁访问任务队列
    pthread_mutex_lock(&thread->lock);
    int solve_cnt = thread->curCount;
    if (solve_cnt == thread->maxCount) {
      end_t = now();
      double sec = (end_t - start_t) / 1000000.0;
      cout << "deal over!!! time: " << sec << endl;
      break;               // 其它线程拿不到锁了，， 所以over
    }
    thread->curCount++;
    pthread_mutex_unlock(&thread->lock);
    //执行任务, 这里是可以有返回值的
    thread->task(solve_cnt);
    // 执行完计算任务，自动回去！
  }
}
