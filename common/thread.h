// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __THREAD_H__
#define __THREAD_H__

#include "noncopyable.h"
#include <iostream>
#include <string>
#include <functional>
#include <thread>

class Thread : public NonCopyable {
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(ThreadFunc thread_func,
                    const std::string &thread_name = std::string());
    ~Thread();
    // std::thread::id get_id();
    void start();
    void join();
    void detach();
    void swap(std::thread &other);
    const std::string& name() const;

private:
    bool started_;
    ThreadFunc thread_func_;
    std::string name_;

    std::thread thread_;
};


#endif /* __THREAD_H__ */