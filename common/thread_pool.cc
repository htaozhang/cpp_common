// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#include "thread_pool.h"

#include <cassert>
#include <iostream>

ThreadPool::ThreadPool(const std::string &name)
    : running_(false),
      max_queue_size_(0),
      name_(name) {

}
ThreadPool::~ThreadPool() {
    if (running_) {
        Stop();
    }
    // std::cout << "done ThreadPool::~ThreadPool" << std::endl;
}

void ThreadPool::SetMaxQueueSize(size_t size) {
    max_queue_size_ = size;
}

size_t ThreadPool::GetQueueSize() {
    std::unique_lock<std::mutex> lock(mutex_);
    return tasks_.size();
}

void ThreadPool::Start(int thread_num) {
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(thread_num);
    for (int i = 0; i < thread_num; i++) {
        threads_.emplace_back(
            new Thread(std::bind(&ThreadPool::RunInThread, this),
                       name_ + std::to_string(i+1)));
        threads_[i]->start();
    }
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        running_ = false;
        not_empty_.notify_all();
    }
    for (auto&& thread : threads_) {
        thread->join();
    }
}

void ThreadPool::Run(task_t task) {
    if (threads_.empty()) {
        task();
    } else {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [&]{ return !IsFull(); });
        tasks_.emplace_back(task);
        not_empty_.notify_one();
    }
}

void ThreadPool::RunInThread() {
  try {
    while (running_) {
      task_t task = nullptr;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [&]{ return !running_ || !tasks_.empty(); });
        if (running_ && !tasks_.empty()) {
          task = tasks_.front();
          tasks_.pop_front();
          not_full_.notify_one();
        }
      }
      if (task) {
        task();
      }
    }
  } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
  }
}

bool ThreadPool::IsFull() const {
    return max_queue_size_ > 0 && tasks_.size() >= max_queue_size_;
}
