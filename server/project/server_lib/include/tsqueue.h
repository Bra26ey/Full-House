#pragma once

#include <queue>
#include <mutex>

template <class T>
class TSQueue {
 public:
    TSQueue();
    ~TSQueue();
    void Push(T &element);
    T Pop();
    bool IsEmpty();

 private:
    std::queue<T> queue_;
    std::mutex mutex_;
};