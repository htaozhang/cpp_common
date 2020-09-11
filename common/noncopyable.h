// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

#include "macros.h"

class NonCopyable {
public:
    DISALLOW_COPY_AND_ASSIGN(NonCopyable);
    DISALLOW_MOVE_COPY_AND_ASSIGN(NonCopyable);

protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
};

#endif /* __NONCOPYABLE_H__ */
