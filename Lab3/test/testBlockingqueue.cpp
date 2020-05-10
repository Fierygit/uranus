//
// Created by Firefly on 2020/5/10.
//
#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>

#include "../coordinator/BoundedBlockingQueue.h"
#include "../common/loguru.hpp"

std::vector<int> testVec;

void putTest(BoundedBlockingQueue<int>& queueTest, int x) {
    for (int i = 0; i < x; i++) {
//        定时
//        std::this_thread::sleep_for(std::chrono::milliseconds (5));
        queueTest.put(i);
    }
}

void getTest(BoundedBlockingQueue<int>& queueTest, int x) {
    for (int i = 0; i < x; i++) {
//         定时
//        std::this_thread::sleep_for(std::chrono::milliseconds(4));
        auto get_val = queueTest.get();
        testVec.push_back(get_val);
//        std::cout << get_val << std::endl;
    }
}


int main() {
    BoundedBlockingQueue<int> queueTest(10);

    // 测试的数据量
    int test_size = 1000;

    std::thread putThread(putTest, std::ref(queueTest), test_size);
    std::thread getThread(getTest, std::ref(queueTest), test_size);

    putThread.join();
    getThread.join();

    // 取出的数量是否正确
    assert(testVec.size() == test_size);
    
    for (int i = 0; i < testVec.size() - 1; i++) {
        // 取出的数据顺序是否正确
        assert(testVec[i + 1] - testVec[i] == 1);
    }

    std::cout << "test pass..." << std::endl;
    return 0;
}