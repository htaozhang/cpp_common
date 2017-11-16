// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

class NonCopyable {
public:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif /* __NONCOPYABLE_H__ */
