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



template <class T>
TSQueue<T>::TSQueue() {}

template <class T>
TSQueue<T>::~TSQueue() {}

template <class T>
void TSQueue<T>::Push(T &element) {}

template <class T>
T TSQueue<T>::Pop() {
    auto tmp = queue_.front();
    queue_.pop();
    return tmp;
}

template <class T>
bool TSQueue<T>::IsEmpty() {
    return true;
}
