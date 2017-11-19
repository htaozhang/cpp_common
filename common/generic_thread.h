// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __GENERIC_THREAD_H__
#define __GENERIC_THREAD_H__


#define THREADNAME_MAXLEN 16

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <cstdio>
#include <cstring>
#include <cstdlib>  // _TRUNCATE
#include <windows.h>
//#define __thread __declspec(thread)

inline char* get_thread_name() {
    __declspec(thread) static char tname[THREADNAME_MAXLEN + 1] = { 0 };
    if (tname[0] == 0) {
        snprintf(tname,
                 THREADNAME_MAXLEN + 1,
                 "%0*X",
                 THREADNAME_MAXLEN,
                 GetCurrentThreadId());
    }
    return &tname[0];
}

inline void set_thread_name(const char* name) {
    //strncpy(get_thread_name(), name, THREADNAME_MAXLEN + 1);
    strncpy_s(get_thread_name(),
              THREADNAME_MAXLEN + 1,
              name,
              _TRUNCATE);
}

inline unsigned long get_thread_id() {
    return GetCurrentThreadId();
}

#elif defined(__linux__) || defined(__APPLE__)
#include <pthread.h>

static pthread_once_t _pthread_key_once = PTHREAD_ONCE_INIT;
static pthread_key_t _pthread_key_name;
void init_pthread_key_name(void) {
    (void)pthread_key_create(&_pthread_key_name, free);
}

inline const char* get_thread_name() {
    auto thread = pthread_self();
    (void)pthread_once(&_pthread_key_once, init_pthread_key_name);
    const char* res = static_cast<const char*>(pthread_getspecific(_pthread_key_name));
    if (res == nullptr) {
        __thread static char tname[THREADNAME_MAXLEN + 1] = { 0 };
        
#ifdef __APPLE__
        uint64_t threadid;
        pthread_threadid_np(thread, &threadid);
#else
        uint64_t threadid = thread;
#endif
        snprintf(tname,
                 THREADNAME_MAXLEN + 1,
                 "%0*X",
                 THREADNAME_MAXLEN,
                 static_cast<unsigned int>(threadid));
        return tname;
    }
    return res;
}

inline void set_thread_name(const char* tname) {
    (void)pthread_once(&_pthread_key_once, init_pthread_key_name);
    (void)pthread_setspecific(_pthread_key_name, strdup(tname));
}

#endif


#endif // !__GENERIC_THREAD_H__


