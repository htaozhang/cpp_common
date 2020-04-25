// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __BLOCKING_QUEURE_H__
#define __BLOCKING_QUEURE_H__

#include "noncopyable.h"

#include <deque>
#include <mutex>
#include <condition_variable>

template<typename T>
class BlockingQueue : public NonCopyable {
public:
    BlockingQueue() = default;
    ~BlockingQueue() = default;

    void Enqueue(const T &one) {
        std::unique_lock<std::mutex> lock(mutex_);
        data_.emplace_back(one);
        not_empty_.notify_all();
    }

    T Dequeue() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [&]{ return data_.size() > 0; });
        T one(data_.front());
        data_.pop_front();
        return one;
    }

    size_t Size() {
        std::unique_lock<std::mutex> lock(mutex_);
        return data_.size();
    }
private:
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::deque<T> data_;
};
#endif /* __BLOCKING_QUEURE_H__ */