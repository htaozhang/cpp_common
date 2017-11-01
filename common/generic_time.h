// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __GENERIC_TIME_H__
#define __GENERIC_TIME_H__

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)


#include <iostream>
#include <sstream>
#include <iomanip>  /* std::get_time */

#define WIN32_LEAN_AND_MEAN
#include <time.h>
#include <windows.h>
#include <winsock2.h>

/*
typedef struct timeval {
long tv_sec;
long tv_usec;
} timeval;
*/

inline int gettimeofday(struct timeval * tp, struct timezone * tzp) {
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}

#define timegm _mkgmtime
#define usleep Sleep

inline char *strptime(const char *s, const char *format, struct tm *tm) {
    std::istringstream ss(s);
    ss.imbue(std::locale(setlocale(LC_ALL, NULL)));
    ss >> std::get_time(tm, format);
    return (ss.fail() ? NULL : const_cast<char*>(s + ss.tellg()));
}

#else
#include <sys/time.h>

#endif


#endif /* __GENERIC_TIME_H__ */