
#include <unistd.h>
#include <iostream>
#include <vector>

#include "time_wrapper.h"
#include "gtest/gtest.h"

using namespace std;

TEST(TimeWrapper, Cost) {
    TimeWrapper timewrapper;
    timewrapper.Start();
    usleep(1000 * 1000);
    EXPECT_EQ(1, timewrapper.Cost() / 1000);
}

TEST(TimeWrapper, ToString) {
    // Beijing: UTC+8, 2017-01-01 08:00:00
    TimeWrapper timewrapper(1LL * 1483228800 * 1000000);
    EXPECT_EQ(string("1483228800000000"), timewrapper.ToString(""));
    EXPECT_EQ(string("20170101"), timewrapper.ToString("%Y%m%d"));
    EXPECT_EQ(string("20170101 00:00:00"), timewrapper.ToString("%Y%m%d %H:%M:%S"));
    EXPECT_EQ(string("2017-01-01T00:00:00"), timewrapper.ToString("%Y-%m-%dT%H:%M:%S"));

}

TEST(TimeWrapper, ToTime) {
    // Beijing: UTC+8, 2017-01-01 08:00:00
    TimeWrapper timewrapper;
    EXPECT_EQ(1483228800, timewrapper.ToTime("2017-01-01 00:00:00", "%Y-%m-%d %H:%M:%S"));
}

TEST(TimeWrapper, SecondsSinceEpoch) {
    // Beijing: UTC+8, 2017-01-01 08:00:00
    TimeWrapper timewrapper(1LL * 1483228800 * 1000000);
    EXPECT_EQ(1483228800, timewrapper.SecondsSinceEpoch());
}

TEST(TimeWrapper, Operator) {
    TimeWrapper lhs(1000000);
    TimeWrapper rhs(2000000);
    TimeWrapper tmp(2000000);
    EXPECT_LT(lhs, rhs);
    EXPECT_NE(lhs, rhs);
    EXPECT_EQ(rhs, tmp);
    EXPECT_EQ((lhs + static_cast<double>(1)), rhs);
}


TEST(TimeWrapper, Benchmark) {
    size_t n = 1000 * 1000;
    vector<TimeWrapper> times;
    times.reserve(n);
    for (size_t i = 0 ; i < n; i++)
        times.push_back(TimeWrapper::Now());

    //printf("%s\n", times.front().ToString().c_str());
    //printf("%s\n", times.back().ToString().c_str());
    //printf("%f\n", times.back() - times.front());
}

