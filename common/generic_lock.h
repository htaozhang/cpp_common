// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __GENERIC_LOCK_H__
#define __GENERIC_LOCK_H__

#include <atomic>
#include <mutex>
#include <thread>

enum class Mutex {};
enum class SpinByTAS {};
enum class SpinByCAS {};

template<typename T>
class GenericLock {
public:
    explicit GenericLock() {}
    ~GenericLock() {}
    void lock() {}
    void unlock() {}
};

template<>
class GenericLock<SpinByTAS> {
public:
    GenericLock(size_t sleep_micro_sec = -1)
        : sleep_micro_sec_(sleep_micro_sec) {
    }

    ~GenericLock() {}

    void lock() {
        while (lock_tas_.test_and_set()) {
            if (sleep_micro_sec_ <= 0) {
                std::this_thread::yield();
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(sleep_micro_sec_));
            }
        }
    }

    void unlock() {
        lock_tas_.clear();
    }

private:
    size_t sleep_micro_sec_;
    std::atomic_flag lock_tas_ = ATOMIC_FLAG_INIT;
};

template<>
class GenericLock<SpinByCAS> {
public:
    GenericLock(size_t sleep_micro_sec = -1)
        : sleep_micro_sec_(sleep_micro_sec) {
    }

    ~GenericLock() {}

    void lock() {
        bool exp = false;
        while (lock_cas_.compare_exchange_strong(exp, true)) {
            if (sleep_micro_sec_ <= 0) {
                std::this_thread::yield();
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(sleep_micro_sec_));
            }
        }
    }

    void unlock() {
        lock_cas_.store(false);
    }

private:
    size_t sleep_micro_sec_;
    std::atomic<bool> lock_cas_ = ATOMIC_VAR_INIT(false);
};

template<>
class GenericLock<Mutex> {
public:
    GenericLock() {}
    ~GenericLock() {}
    void lock() { lock_.lock(); }
    void unlock() { lock_.unlock(); }

private:
    std::mutex lock_;
};

#endif /* __GENERIC_LOCK_H__ */
