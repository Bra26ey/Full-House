#pragma once

#include <vector>
#include <mutex>

template <class T>
class TSVector {
 public:
    TSVector() = default;
    ~TSVector();
    void PushBack(T &element);
    void PopBack();
    bool IsEmpty();
    T At(size_t position);

 private:
    std::vector<T> vector_;
    std::mutex mutex_;
};


template <class T>
TSVector<T>::~TSVector() {
    mutex_.lock();
}

template <class T>
void TSVector<T>::PushBack(T &element) {
    if (vector_.capacity() <= vector_.size()) {
        mutex_.lock();
    }
    vector_.push_back(element);
    mutex_.unlock();
}

template <class T>
void TSVector<T>::PopBack() {
    vector_.pop_back()
}

template <class T>
bool TSVector<T>::IsEmpty() {
    mutex_.lock();
    bool result = vector_.empty();
    mutex_.unlock();
    return result;
}

template <class T>
T TSVector<T>::At(size_t position) {
    return vector_.at(position);
}

