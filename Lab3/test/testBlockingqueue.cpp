//
// Created by Firefly on 2020/5/10.
//
#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include <syslog.h>

#include "../coordinator/BoundedBlockingQueue.h"
#include "../common/loguru.hpp"

std::vector<int> testVector;
std::mutex g_mutex{};

void putTest(BoundedBlockingQueue<int>& queueTest, int x) {
    for (int i = 1; i <= x; i++) {
//        定时
//        std::this_thread::sleep_for(std::chrono::milliseconds (5));
        queueTest.put(i);
    }
}

void getTest(BoundedBlockingQueue<int>& queueTest, int x) {
    for (int i = 1; i <= x; i++) {
//         定时
//        std::this_thread::sleep_for(std::chrono::milliseconds(4));
// 要加锁, 保证原子操作

        std::unique_lock<std::mutex> lck(g_mutex);
        auto get_val = queueTest.get();
        testVector.emplace_back(get_val);
//        std::cout << get_val << std::endl;
    }
}

void oneToOneTest() {
    std::cout << "oneToOneTest start ..." << std::endl;

    BoundedBlockingQueue<int> queueTest(10);
    testVector.clear();

    // 测试的数据量
    int test_size = 10000;

    std::thread putThread(putTest, std::ref(queueTest), test_size);
    std::thread getThread(getTest, std::ref(queueTest), test_size);

    putThread.join();
    getThread.join();

    // 取出的数量是否正确
    assert(testVector.size() == test_size);

    for (int i = 0; i < testVector.size() - 1; i++) {
        // 取出的数据顺序是否正确
        assert(testVector[i + 1] - testVector[i] == 1);
    }

    std::cout << "oneToOneTest pass..." << std::endl;
}

void multiToMultiTest() {
    std::cout << "multiToMultiTest start ..." << std::endl;

    BoundedBlockingQueue<int> queueTest(1024);
    testVector.clear();

    int threads_cnt = 100;
    int test_size = 5000;

    std::vector<std::thread> putThreads;
    std::vector<std::thread> getThreads;

    for (int i = 0; i < threads_cnt; i++) {
        putThreads.emplace_back(std::thread(putTest, std::ref(queueTest), test_size));
        getThreads.emplace_back(std::thread(getTest, std::ref(queueTest), test_size));
    }

    for (int i = 0; i < threads_cnt; i++) {
        putThreads[i].join();
        getThreads[i].join();
    }
//    std::cout << testVector.size() << std::endl;
    // 取出的数量是否正确
    assert(testVector.size() == threads_cnt * test_size);

    int should_sum = threads_cnt * (((1 + test_size) * test_size) / 2);
    int sum = 0;
    for (int i = 0; i < testVector.size(); i++) {
        // 取出的数据顺序是否正确
        sum += testVector[i];
    }
//    std::cout << sum << std::endl;
    // 所有数的和
    assert(sum == should_sum);

    std::cout << "multiToMultiTest pass..." << std::endl;
}


int main() {
    oneToOneTest();
    multiToMultiTest();
    return 0;
}