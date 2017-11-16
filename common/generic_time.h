// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __GENERIC_TIME_H__
#define __GENERIC_TIME_H__

#include "utils.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <iostream>
#include <sstream>
#include <iomanip>  // std::get_time
#include <ctime>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>   // struct timeval

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

inline char *strptime(const char *s, const char *format, struct tm *tm) {
    std::istringstream ss(s);
    ss.imbue(std::locale(setlocale(LC_ALL, NULL)));
    ss >> std::get_time(tm, format);
    return (ss.fail() ? NULL : const_cast<char*>(s + static_cast<int>(ss.tellg())));
}

#define timegm _mkgmtime

#define usleep(x) Sleep((x) / 1000)


#else
#include <sys/time.h>

#endif

inline utils::Null<> localtime_r(...) { return utils::Null<>(); }
inline utils::Null<> localtime_s(...) { return utils::Null<>(); }
inline utils::Null<> gmtime_r(...) { return utils::Null<>(); }
inline utils::Null<> gmtime_s(...) { return utils::Null<>(); }

inline std::tm localtime(std::time_t time) {
    struct LocalTime {
        std::time_t time_;
        std::tm tm_;

        LocalTime(std::time_t t) : time_(t) {}

        bool run() { return handle(localtime_r(&time_, &tm_)); }
        bool handle(std::tm *tm) { return tm != NULL; }
        bool handle(utils::Null<>) { return fallback(localtime_s(&tm_, &time_)); }
        bool fallback(int res) { return res == 0; }
        bool fallback(utils::Null<>) {
            std::tm *tm = std::localtime(&time_);
            return tm == NULL ? false : (tm_ = *tm, true);
        }
    };

    LocalTime lt(time);
    return lt.run() ? lt.tm_ : std::tm();
}

inline std::tm gmtime(std::time_t time) {
    struct GMTime {
        std::time_t time_;
        std::tm tm_;

        GMTime(std::time_t t) : time_(t) {}

        bool run() { return handle(gmtime_r(&time_, &tm_)); }
        bool handle(std::tm *tm) { return tm != NULL; }
        bool handle(utils::Null<>) { return fallback(gmtime_s(&tm_, &time_)); }
        bool fallback(int res) { return res == 0; }
        bool fallback(utils::Null<>) {
            std::tm *tm = std::gmtime(&time_);
            return tm == NULL ? false : (tm_ = *tm, true);
        }
    };

    GMTime gt(time);
    return gt.run() ? gt.tm_ : std::tm();
}

#endif /* __GENERIC_TIME_H__ */
