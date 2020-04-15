#include "ThreadPool.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace uranus {
// 构造函数 - 设置线程数量
ThreadPool::ThreadPool(int threadNum) {
	isRunning_	= true;
	threadsNum_ = threadNum;
	createThreads();
}

ThreadPool::~ThreadPool() {
	stop();
}

// 创建线程, 线程执行threadFunc函数, 该函数会循环(阻塞)地执行take, 获取新任务执行
int ThreadPool::createThreads() {
	pthread_mutex_init(&mutex_, NULL);
	pthread_cond_init(&condition_, NULL);
	// 线程数组
	threads_ = (pthread_t*)malloc(sizeof(pthread_t) * threadsNum_);
	for (int i = 0; i < threadsNum_; i++) {
		pthread_create(&threads_[i], NULL, threadFunc, this);
	}
	return 0;
}

// 添加任务. 参数是一个 Function对象
size_t ThreadPool::addTask(const Task& task) {
	pthread_mutex_lock(&mutex_);
	taskQueue_.push_back(task);
	int size = taskQueue_.size();
	pthread_mutex_unlock(&mutex_);
	// ! 当添加一个任务的时候, 唤醒一个阻塞在该条件变量的线程
	pthread_cond_signal(&condition_);
	return size;
}

// 停止线程
void ThreadPool::stop() {
	if (!isRunning_) {
		return;
	}

	isRunning_ = false;
	// ANCHOR 唤醒所有被阻塞的线程.
	// 因为设置了 isRunning = false, 所以线程醒了之后会跳出循环然后结束线程
	pthread_cond_broadcast(&condition_);

	// 线程执行完成
	for (int i = 0; i < threadsNum_; i++) {
		pthread_join(threads_[i], NULL);
	}

	free(threads_);
	threads_ = NULL;

	pthread_mutex_destroy(&mutex_);
	pthread_cond_destroy(&condition_);
}

// 当前任务数量
int ThreadPool::size() {
	pthread_mutex_lock(&mutex_);
	int size = taskQueue_.size();
	pthread_mutex_unlock(&mutex_);
	return size;
}

// 拿任务, 如果running且没任务则阻塞, 如果不running则
ThreadPool::Task ThreadPool::take() {
	Task task = NULL;
	pthread_mutex_lock(&mutex_);
	// 取任务的时候, 如果队列为空, 并且正在运行, 那么等待条件
	while (taskQueue_.empty() && isRunning_) {
		pthread_cond_wait(&condition_, &mutex_);
	}

	if (!isRunning_) {
		pthread_mutex_unlock(&mutex_);
		return task;
	}

	assert(!taskQueue_.empty());
	task = taskQueue_.front();
	taskQueue_.pop_front();
	pthread_mutex_unlock(&mutex_);
	return task;
}

// 线程执行的函数, 会取任务执行
void* ThreadPool::threadFunc(void* arg) {
	pthread_t tid	 = pthread_self();
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	while (pool->isRunning_) {
		// 取不到就在条件变量那里阻塞
		ThreadPool::Task task = pool->take();
		if (!task) {
			printf("thread %lu will exit\n", tid);
			break;
		}

		assert(task);
		task();
	}
	return 0;
}

}  // namespace uranus
