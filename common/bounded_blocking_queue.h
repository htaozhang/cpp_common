// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __BOUNDED_BLOCKING_QUEURE_H__
#define __BOUNDED_BLOCKING_QUEURE_H__

#include "noncopyable.h"

#include <deque>
#include <mutex>

template<typename T>
class BoundedBlockingQueue : public NonCopyable {
public:
    BoundedBlockingQueue(int capaticy)
        : capacity_(capaticy) {
    }

    ~BoundedBlockingQueue() = default;

    void Put(const T &one) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [&]{ return data_.size() < capacity_; });
        
        data_.emplace_back(one);
        not_empty_.notify_all();
    }

    T Get() {
        std::unique_lock<std::mutex> lock(mutex_);
        no_empty_.wait(lock, [&]{ return data_.size() > 0; });
        T one(data_.front());
        data_.pop_front();
        no_full_.notify_all();
        return one;
    }

    size_t Size() {
        std::unique_lock<std::mutex> lock(mutex_);
        return data_.size();
    }

    size_t Capacity() {
        // std::unique_lock<std::mutex> lock(mutex_);
        return capacity_;
    }
private:
    int capacity_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    sd::deque<T> data_;
};
#endif /* __BOUNDED_BLOCKING_QUEURE_H__ */