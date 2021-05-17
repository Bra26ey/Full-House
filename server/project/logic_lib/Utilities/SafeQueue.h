//
// Created by aleksandr on 28.12.2020.
//

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace logic {

template <class T>
class SafeQueue {
public:
    SafeQueue(): q(), m(), c() {}

    ~SafeQueue() {}

    // Add an element to the queue.
    void push(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T pop() {
        std::unique_lock<std::mutex> lock(m);
        while(q.empty()) {
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

}  // namespace logic
