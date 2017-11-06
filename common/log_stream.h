// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __LOG_STREAM_H__
#define __LOG_STREAM_H__

#include <string>
#include <utility>
#include <algorithm> // std::fill

#include <stdio.h>
#include <string.h> // memcpy() strlen()

#include "utils.h"
#include "noncopyable.h"

const int kDefaultBuffer = 4096;
const int kMaximumBuffer = 4096 * 1024;

class Format {
public:
    template<typename T>
    Format(const char* fmt, T x);

    const char* Data() const { return data_; }
    int Length() const { return length_; }
private:
    char data_[32];
    int length_;
};

template<int SIZE = kDefaultBuffer>
class Buffer : public NonCopyable {
public:
    Buffer() : cur_(data_) {}
    ~Buffer() {}
    void Append(const char* from, size_t len) {
        if (utils::implicit_cast<size_t>(Usable()) > len) {
            memcpy(cur_, from, len);
            cur_ += len;
        }
    }

    void Add(size_t len) { cur_ += len; }
    const char* Data() const { return data_; }
    int Length() const { return static_cast<int>(cur_ - data_); }
    int Usable() { return static_cast<int>(data_ + sizeof(data_) - cur_); }
    char* Current() { return cur_; }
    void Reset() { cur_ = data_; }
    //void Clear() { ::bzero(data_, sizeof data_); }
    void Clear() { std::fill(data_, data_ + SIZE, 0); }
    const std::string ToString() const { return std::string(data_, Length()); }
    std::string ToString() { return std::string(data_, Length()); }

private:
    char data_[SIZE];
    char* cur_;
};


class LogStream : public NonCopyable {
public:
    LogStream() {}
    ~LogStream() {}

    LogStream& operator<<(bool);
    LogStream& operator<<(char);
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(float);
    LogStream& operator<<(double);
    LogStream& operator<<(const void*);
    LogStream& operator<<(const char*);
    LogStream& operator<<(const unsigned char*);
    LogStream& operator<<(const std::string&);
    LogStream& operator<<(const Buffer<kDefaultBuffer>&);
    LogStream& operator<<(const Format&);
    LogStream& operator<<(const std::pair<const char*, size_t>&);

    void Append(const char* data, size_t len) { buffer_.Append(data, len); }
    const Buffer<kDefaultBuffer>& Cache() const { return buffer_; }
    void Reset() { buffer_.Reset(); }

private:
    Buffer<kDefaultBuffer> buffer_;
    static const int kMaxNumericSize = 32;
};

#endif /* __LOG_STREAM_H__ */



