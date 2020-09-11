// Copyright(c) 2020 macrozhang
// All rights reserved.
//
//
#pragma once


#define DISALLOW_COPY(cls) cls(const cls&) = delete;

#define DISALLOW_MOVE_COPY(cls) cls(const cls&&) = delete;
    
#define DISALLOW_ASSIGN(cls) cls& operator=(const cls&) = delete;

#define DISALLOW_MOVE_ASSIGN(cls) cls& operator=(const cls&&) = delete;


#define DISALLOW_COPY_AND_ASSIGN(cls) \
    DISALLOW_COPY(cls);               \
    DISALLOW_ASSIGN(cls);

#define DISALLOW_MOVE_COPY_AND_ASSIGN(cls)  \
    DISALLOW_MOVE_COPY(cls);                \
    DISALLOW_MOVE_ASSIGN(cls);


template<typename T>
inline void IGNORE_RESULT(const T&) {}