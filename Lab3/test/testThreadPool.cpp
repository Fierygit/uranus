//
// Created by Firefly on 2020/5/17.
//

#include <iostream>
#include <random>
#include <zconf.h>
#include "../common/ThreadPool.h"
#include "../common/WaitGroup.h"

int main() {

    std::cout << "start test" << std::endl;
    WaitGroup waitGroup;
    int cnt = 5;
    waitGroup.Add(cnt);
    uranus::ThreadPool threadPool;
    std::default_random_engine random;
    for (int i = 0; i < cnt; i++) {
        threadPool.addTask([&] {
            int w = random() % 6;   // 执行太快了都获取不到随机数
            //sleep(w);
            printf("%d wait %d  over\n", i, w);
            waitGroup.Done();
        });
        //sleep(random() % 1); // 为了获取一个上面的随机数
    }
    waitGroup.Wait();
    threadPool.stop();
    printf("over\n");
}