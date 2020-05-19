//
// Created by Firefly on 2020/5/17.
//

#ifndef LAB3_WAITGROUP_H
#define LAB3_WAITGROUP_H

#include <mutex>
#include <atomic>
#include <condition_variable>

class WaitGroup {
public:
    void Add(int incr = 1) {
        counter += incr;
        LOG_F(INFO,"WaitGroup num : %d ", incr);
    }

    void Done() {
        counter--;
        if (counter <= 0)
            cond.notify_all();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [&] { return counter <= 0; });
        LOG_F(INFO,"WaitGroup over ");
    }

private:
    std::mutex mutex;
    std::atomic<int> counter{0};
    std::condition_variable cond;
};


#endif //LAB3_WAITGROUP_H
