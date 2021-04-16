#pragma once

#include <queue>
#include <mutex>

template <class T>
class TSQueue {
 public:
    Queue();
    ~Queue();
    void Push(T &element);
    T Pop();
    bool IsEmpty();

 private:
    std::queue<T> queue_;
    std::mutex mutex_;
};