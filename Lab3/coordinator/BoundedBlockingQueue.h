#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H


#include <assert.h>
#include <mutex>
#include <condition_variable>


/*
 * 把生产者与消费者问题的buf 换成队列， 注意要线程安全
 * */

template<typename T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(int maxSize) {
        std::cout << maxSize << std::endl;
    }


    void put(const T &x) {}

    void put(T &&x) {}

    T take() {}

    bool empty() const {}

    bool full() const {}

    size_t size() const {}

    //
    size_t capacity() const {
        std::lock_guard<std::mutex> lock_guard(mutex);
        return 1;
    }

private:
    //锁
    std::mutex mutex{};
    //条件变量
    std::condition_variable notEmpty;
    std::condition_variable notFull;


};


#endif
