// Copyright(c) 2017 macrozhang.
// All rights reserved.
//
#ifndef __TIME_WRAPPER_H__
#define __TIME_WRAPPER_H__


#include <string>
#include <cstdio> 
#include <functional>
#include <sys/time.h>
#include <cinttypes>

using namespace std;


class TimeWrapper
{
public:
    TimeWrapper();
    explicit TimeWrapper(int64_t microSecondsSinceEpoch);
    ~TimeWrapper();

    void Start();
    void Stop();
    int64_t Cost(const string& unit = "ms");
    int64_t MicroSecondsSinceEpoch() const;
    time_t SecondsSinceEpoch() const;
    string ToString(const string& fmt = "%Y%m%d %H:%M:%S", bool showMicroSec = true) const;
    time_t ToTime(const string& timeString, const string& fmt) const;

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
