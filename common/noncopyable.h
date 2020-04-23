// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

class NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete;
    void operator=(const NonCopyable&) = delete;
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

#endif /* __NONCOPYABLE_H__ */
