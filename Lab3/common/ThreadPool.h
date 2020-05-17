#pragma once
#include <pthread.h>

#include <deque>
#include <functional>  // for std::function, std::bind

#define ThreadNum 10

// 每一个job都是一个function对象
namespace uranus {
class ThreadPool {
   public:
	// 一个任务是一个函数
	typedef std::function<void()> Task;

	ThreadPool(int threadNum = ThreadNum);
	~ThreadPool();

	size_t addTask(const Task& task);
	void stop();
	int size();
	Task take();

   private:
	int createThreads();
	static void* threadFunc(void* threadData);

	ThreadPool& operator=(const ThreadPool&);
	ThreadPool(const ThreadPool&);

	volatile bool isRunning_;
	int threadsNum_;
	pthread_t* threads_;

	// 任务队列
	std::deque<Task> taskQueue_;
	pthread_mutex_t mutex_;
	pthread_cond_t condition_;
};
}  // namespace uranus
