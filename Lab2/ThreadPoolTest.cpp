#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "ThreadPool.h"

// 任务类
class MyTask {
   public:
	MyTask() {}

	int run(int i, const char* p) {
		printf("thread[%lu] : (%d, %s)\n", pthread_self(), i, (char*)p);
		sleep(2);
		return 0;
	}
};

int main() {
	uranus::ThreadPool threadPool(4);
	MyTask taskObj[20];
	for (int i = 0; i < 20; i++) {
		// bind是函数适配器, 现在(加入队列时)绑定参数
		// 添加类方法方式
		threadPool.addTask(std::bind(&MyTask::run, &taskObj[i], i, "hello, world"));
		// 添加普通方法方式
		threadPool.addTask(std::bind(
			[](int i, std::string str) {
				printf("fuck thread[%lu] : (%d, %s)\n", pthread_self(), i, (char*)str.c_str());
				sleep(1);
			},
			i, "fuck it"));
	}

	while (1) {
		printf("there are still %d tasks need to process\n", threadPool.size());
		// 等待队列结束
		if (threadPool.size() == 0) {
			threadPool.stop();
			printf("Now I will exit from main\n");
			exit(0);
		}
		sleep(2);
	}

	return 0;
}
