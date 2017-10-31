// Copytright(c) 2017 macrozhang.
// All rights reserved.
//
#include "time_wrapper.h"

TimeWrapper::TimeWrapper()
    : microSecondsSinceEpoch_(0) {
}

TimeWrapper:: TimeWrapper(int64_t microSecondsSinceEpoch)
    : microSecondsSinceEpoch_(microSecondsSinceEpoch) {
}

TimeWrapper::~TimeWrapper() {
}

int64_t TimeWrapper::MicroSecondsSinceEpoch() const {
    return microSecondsSinceEpoch_;
}

time_t TimeWrapper::SecondsSinceEpoch() const {
    return static_cast<time_t>(microSecondsSinceEpoch_ / microSecondsPerSecond);
}

string TimeWrapper::ToString(const string& fmt/* = "%Y%m%d %H:%M:%S" */, bool showMicroSec/* = true */) const {
    char buf[32] = {0};
    
    if (fmt.empty()) {
        int64_t seconds = microSecondsSinceEpoch_ / microSecondsPerSecond;
        int64_t microseconds = microSecondsSinceEpoch_ % microSecondsPerSecond;
        snprintf(buf, sizeof(buf)-1, "%" PRId64 "%.06" PRId64 "", seconds, microseconds);
        return buf;
    } else {
        time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / microSecondsPerSecond);
        struct tm *TM = gmtime(&seconds);
        strftime(buf, sizeof(buf)-1, fmt.c_str(), TM);
        return buf;
    }

    return "";
}

time_t TimeWrapper::ToTime(const string& timeString, const string& fmt) const {
    struct tm TM;
    strptime(timeString.c_str(), fmt.c_str(), &TM);
    TM.tm_sec = 0;
    //time_t result = mktime(&TM);
    time_t result = timegm(&TM);  // UTC-0
    return result;
}

TimeWrapper TimeWrapper::Now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return TimeWrapper(seconds * microSecondsPerSecond + tv.tv_usec);
}

TimeWrapper TimeWrapper::FromUnixTime(time_t t) {
    return FromUnixTime(t, 0);
}

TimeWrapper TimeWrapper::FromUnixTime(time_t t, int64_t microSeconds) {
    return TimeWrapper(static_cast<int64_t>(t) * microSecondsPerSecond + microSeconds);
}

void TimeWrapper::Start() {
    gettimeofday(&start_, NULL);
}

void TimeWrapper::Stop() {
    gettimeofday(&stop_, NULL);
}

int64_t TimeWrapper::Cost(const string& unit/* = "s" */) {
    //gettimeofday(&stop_, NULL);
    Stop();

    if (unit == "ms")
        return (stop_.tv_sec - start_.tv_sec) * 1000 + (stop_.tv_usec - start_.tv_usec) / 1000;
    
    return (stop_.tv_sec - start_.tv_sec) + (stop_.tv_usec - start_.tv_usec) / microSecondsPerSecond;
}

void TimeWrapper::Timeit(function<void()> func) {
    struct timeval from;
    struct timeval to;
    gettimeofday(&from, NULL);
    func();
    gettimeofday(&to, NULL);
    printf("%.4f secs\n", 
        (double)(to.tv_sec - from.tv_sec) + (double)(to.tv_usec - from.tv_usec) / microSecondsPerSecond);
}

bool operator<(TimeWrapper lhs, TimeWrapper rhs) {
    return lhs.microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
}

bool operator==(TimeWrapper lhs, TimeWrapper rhs) {
    return lhs.microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
}

bool operator!=(TimeWrapper lhs, TimeWrapper rhs) {
    return lhs.microSecondsSinceEpoch_ != rhs.microSecondsSinceEpoch_;
}

double operator-(TimeWrapper lhs, TimeWrapper rhs) {
    return static_cast<double>(
        lhs.microSecondsSinceEpoch_ - rhs.microSecondsSinceEpoch_) / TimeWrapper::microSecondsPerSecond;
}

TimeWrapper operator+(TimeWrapper lhs, double seconds) {
    int64_t delta = static_cast<int64_t>(seconds * TimeWrapper::microSecondsPerSecond);
    return TimeWrapper(lhs.microSecondsSinceEpoch_ + delta);
}


