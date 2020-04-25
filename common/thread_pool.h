// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "noncopyable.h"
#include "thread.h"

#include <queue>
#include <mutex>
#include <vector>
#include <condition_variable>

class ThreadPool : public NonCopyable {
public:
    typedef std::function<void ()> task_t;

    explicit ThreadPool(const std::string &name = std::string("ThraedPool"));
    ~ThreadPool();

    void SetMaxQueueSize(size_t size);
    size_t GetQueueSize();

    void Start(int thread_num);
    void Stop();
    void Run(task_t task);

private:
    void RunInThread();
    bool IsFull() const;

private:
    bool running_;
    size_t max_queue_size_;
    std::string name_;
    std::vector<std::unique_ptr<Thread>> threads_;
    std::deque<task_t> tasks_;

    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

#endif /* __THREAD_POOL_H__ */