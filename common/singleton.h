// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "noncopyable.h"

template<typename T>
class Singleton : public NonCopyable {
public:
    static T& Instance() {
        pthread_once(&ponce_, &Singleton::Init);
        assert(value_ != nullptr);
        return *value_;
    }

    static void Init() {
        value_ = new T();
    }

private:
    static T *value_;
    static pthread_once_t ponce_;
};

#endif /* __SINGLETON_H__ */
