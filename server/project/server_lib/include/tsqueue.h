#pragma once

#include <queue>
#include <mutex>

template <class T>
class TSQueue {
 public:
    TSQueue() = default;
    ~TSQueue();
    void Push(T &element);
    T Pop();
    bool IsEmpty();

 private:
    std::queue<T> queue_;
    std::mutex mutex_;
};



template <class T>
TSQueue<T>::~TSQueue() {
    mutex_.lock();
}

template <class T>
void TSQueue<T>::Push(T &element) {
    mutex_.lock();
    queue_.push(element);
    mutex_.unlock();
}

template <class T>
T TSQueue<T>::Pop() {
    mutex_.lock();
    auto tmp = queue_.front();
    queue_.pop();
    mutex_.unlock();
    return tmp;
}

template <class T>
bool TSQueue<T>::IsEmpty() {
    mutex_.lock();
    bool result = queue_.empty();
    mutex_.unlock();
    return result;
}
