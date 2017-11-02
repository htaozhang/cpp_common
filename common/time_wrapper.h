// Copyright(c) 2017 macrozhang.
// All rights reserved.
//
#ifndef __TIME_WRAPPER_H__
#define __TIME_WRAPPER_H__

#include <string>
#include <functional>   // function

#include <inttypes.h>

#include "generic_time.h"

class TimeWrapper {
public:
    TimeWrapper();
    explicit TimeWrapper(int64_t microSecondsSinceEpoch);
    ~TimeWrapper();

    void Start();
    void Stop();
    int64_t Cost(const std::string& unit = "ms");
    int64_t MicroSecondsSinceEpoch() const;
    time_t SecondsSinceEpoch() const;
    std::string ToString(const std::string& fmt = "%Y%m%d %H:%M:%S", bool showMicroSec = true) const;
    time_t ToTime(const std::string& timeString, const std::string& fmt) const;

    static TimeWrapper Now();
    static TimeWrapper FromUnixTime(time_t t);
    static TimeWrapper FromUnixTime(time_t t, int64_t microSeconds);
    static void Timeit(std::function<void()> func);

    friend bool operator<(TimeWrapper lhs, TimeWrapper rhs);
    friend bool operator==(TimeWrapper lhs, TimeWrapper rhs);
    friend bool operator!=(TimeWrapper lhs, TimeWrapper rhs);
    
    ///
    /// @return (lhs - rhs) in seconds
    ///
    friend double operator-(TimeWrapper lhs, TimeWrapper rhs);
    
    ///
    /// @return (TimeWrapper + seconds) as TimeWrapper
    /// 
    friend TimeWrapper operator+(TimeWrapper lhs, double seconds);

    static const int microSecondsPerSecond = 1000 * 1000;

private:
    struct timeval start_;
    struct timeval stop_;
    int64_t microSecondsSinceEpoch_;
};

#endif /* __TIME_WRAPPER_H__ */
