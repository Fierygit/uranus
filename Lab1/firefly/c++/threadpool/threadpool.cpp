/*
 * @Author: Firefly
 * @Date: 2020-02-28 10:05:34
 * @Descripttion:
 * @LastEditTime: 2020-02-28 11:22:28
 */
#include "threadpool.h"

//构造函数,确定线程池的线程数
Threadpool::Threadpool(size_t cntThread, size_t maxCount) {
  //初始化条件变量与互斥量
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&null, NULL);

  this->maxCount = maxCount;

  //创建线程
  cout << "main pthread pid is " << ((unsigned)pthread_self() % 100) << endl;
  pthread_t pid[cntThread];
  for (int i = 0; i < cntThread; i++) {
    // 第一个  就是 控制这个线程 句柄上面的数组
    // 第二个 是设置新的线程的，栈大小等等信息，默认为空就可以了
    // 第3个参数是一个函数指针，它返回void * 参数也是void *, 回调函数
    // 第四个 参数是线程的参数  
    int ret = pthread_create(pid + i, NULL, start, this);
    while(ret != 0){
        cout << "create pthread error! " << ret << "\n";
        int ret = pthread_create(pid + i, NULL, start, this);
    }
  }
  
}

//析构函数
Threadpool::~Threadpool(void) {
  //销毁条件变量与互斥量
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&null);
}

//添加任务
bool Threadpool::addTask(Task task) {
  //加锁
  pthread_mutex_lock(&lock);
  //检查任务数量
  //cout << taskQueue.size() << " " << maxCount << endl;
  if (taskQueue.size() >= maxCount) {
    cout << "wiat pool is full" << endl;
    pthread_mutex_unlock(&lock);
    return false;
  }

  //加入任务到队列
  taskQueue.push(task);
  //唤醒空状态进入睡眠的线程
  pthread_cond_broadcast(&null);
  //解锁
  pthread_mutex_unlock(&lock);
}

//线程入口
void* Threadpool::start(void* arg) {
  Threadpool* thread = (Threadpool*)arg;
  cout << "create thread " << ((unsigned)pthread_self() % 100) << endl;
  while (true) {
    //加锁访问任务队列
    pthread_mutex_lock(&thread->lock);
    //Wait for condition variable COND to be signaled or broadcast.
    //MUTEX is assumed to be locked before.
    while (thread->taskQueue.empty()) {
      // while 循环的作用， 醒来时被其它的线程抢走任务， 此时再判断一次
      cout << pthread_self() << ": wait queue is empty!" << endl;
      pthread_cond_wait(&thread->null, &thread->lock);
      // 释放锁
    }

    //从队列中获取任务， 获取任务的函数指针
    Task task = thread->taskQueue.front();
    thread->taskQueue.pop();
    //先解锁，因为不知道任务需要执行多久
    pthread_mutex_unlock(&thread->lock);
    //执行任务, 这里是可以有返回值的
    task();
    // 执行完计算任务，自动回去！
    
  }
}
