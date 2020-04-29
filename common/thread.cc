// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#include "thread.h"

Thread::Thread(ThreadFunc thread_func, const std::string &thread_name)
    : started_(false),
      thread_func_(thread_func),
      name_(thread_name) {
}

Thread::~Thread() {
    detach();
    // std::cout << "done Thread::~Thread" << std::endl;
}

// std::thread::id Thread::get_id() {
//     return thread_.get_id();
// }

void Thread::start() {
    started_ = true;
    thread_ = std::thread(thread_func_);
}

void Thread::join() {
    if (started_ && thread_.joinable()) {
        thread_.join();
    }
}

void Thread::detach() {
    if (started_ && thread_.joinable()) {
        thread_.detach();
    }
}

void Thread::swap(std::thread &other) {
    thread_.swap(other);
}

const std::string& Thread::name() const {
    return name_;
}
