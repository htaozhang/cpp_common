// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#include "log_stream.h"
#include "utils.h"

#include <assert.h>


template<typename T>
Format::Format(const char* fmt, T x) {
    assert(std::is_arithmetic<T>::value == true);
    length_ = static_cast<size_t>(snprintf(data_, sizeof(data_), fmt, x));
    assert(length_ < sizeof(data_));
}

// explcit
template Format::Format(const char* fmt, char);
template Format::Format(const char* fmt, short);
template Format::Format(const char* fmt, unsigned short);
template Format::Format(const char* fmt, int);
template Format::Format(const char* fmt, unsigned int);
template Format::Format(const char* fmt, long);
template Format::Format(const char* fmt, unsigned long);
template Format::Format(const char* fmt, long long);
template Format::Format(const char* fmt, unsigned long long);
template Format::Format(const char* fmt, float);
template Format::Format(const char* fmt, double);


LogStream& LogStream::operator<<(bool x) {
    buffer_.Append(x ? "1" : "0", 1);
    return *this;
}

LogStream& LogStream::operator<<(char x) {
    buffer_.Append(&x, 1);
    return *this;
}


LogStream& LogStream::operator<<(short x) {
    *this << static_cast<int>(x);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short x) {
    *this << static_cast<unsigned int>(x);
    return *this;
}

LogStream& LogStream::operator<<(int x) {
    if (buffer_.Usable() > kMaxNumericSize) 
        buffer_.Add(utils::ConvertToString(buffer_.Current(), x));
    return *this;
}

LogStream& LogStream::operator<<(unsigned int x) {
    if (buffer_.Usable() > kMaxNumericSize) 
        buffer_.Add(utils::ConvertToString(buffer_.Current(), x));
    return *this;
}

LogStream& LogStream::operator<<(long x) {
    if (buffer_.Usable() > kMaxNumericSize) 
        buffer_.Add(utils::ConvertToString(buffer_.Current(), x));
    return *this;
}

LogStream& LogStream::operator<<(unsigned long x) {
    if (buffer_.Usable() > kMaxNumericSize) 
        buffer_.Add(utils::ConvertToString(buffer_.Current(), x));
    return *this;
}


LogStream& LogStream::operator<<(long long x) {
    if (buffer_.Usable() > kMaxNumericSize) 
        buffer_.Add(utils::ConvertToString(buffer_.Current(), x));
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long x){
    if (buffer_.Usable() > kMaxNumericSize) 
        buffer_.Add(utils::ConvertToString(buffer_.Current(), x));
    return *this;
}

LogStream& LogStream::operator<<(float x) {
    *this << static_cast<double>(x);
    return *this;
}

LogStream& LogStream::operator<<(double x) {
    if (buffer_.Usable() > kMaxNumericSize)
        buffer_.Add(static_cast<size_t>(snprintf(buffer_.Current(), kMaxNumericSize, "%.12g", x)));
    return *this;
}

LogStream& LogStream::operator<<(const void* x) {
    std::string str = utils::ConvertHexToString(x);
    *this << str;
    return *this;
}

LogStream& LogStream::operator<<(const char* x) {
    if (x) {
        buffer_.Append(x, strlen(x));
    } else {
        buffer_.Append("(null)", 6);
    }

    return *this;
}

LogStream& LogStream::operator<<(const unsigned char* x) {
    *this << reinterpret_cast<const char*>(x);
    return *this;
}

LogStream& LogStream::operator<<(const std::string& x) {
    buffer_.Append(x.c_str(), x.size());
    return *this;
}


LogStream& LogStream::operator<<(const Buffer<kDefaultBuffer>& x) {
    buffer_.Append(x.Data(), x.Length());
    return *this;
}

LogStream& LogStream::operator<<(const Format& x) {
    buffer_.Append(x.Data(), x.Length());
    return *this;
}

LogStream& LogStream::operator<<(const std::pair<const char*, size_t>& x) {
    buffer_.Append(x.first, x.second);
    return *this;
}
