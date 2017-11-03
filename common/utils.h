// Copyright(c) 2017 macrozhang 
// All rights reserved.
//

#ifndef __UTILS_H__
#define __UTILS_H__

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
//#include <nmmintrin.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wtautological-compare"
#elif defined(__linux__)
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif


namespace utils {
#if !defined(DIGITS)
    #define DIGITS "9876543210123456789"
#endif
#if !defined(DIGITSHEX)
    #define DIGITSHEX "0123456789ABCDEF"
#endif
#if !defined(ZERO)
    #define ZERO (DIGITS + 9)
#endif

//const char digits[] = "9876543210123456789";
//const char digitsHex[] = "0123456789ABCDEF";
//const char* zero = digits + 9;

    template<typename T = void>
    struct Null {};

// boost::implicit_cast
template<typename T> struct identity { typedef T type; };

template <typename T>
inline T implicit_cast(typename identity<T>::type x) {
    return x;
}

template<typename T>
inline std::string ConvertToString(T value) {
    std::string answer;
    T i = value;
    
    do {
        int j = static_cast<int>(i % 10);
        i /= 10;
        answer = answer + ZERO[j];
    } while (i != 0);

    if (value < 0)
        answer = answer + '-';

    std::reverse(answer.begin(), answer.end());

    return answer;
}

template<typename T>
inline size_t ConvertToString(char buff[], T value) {
    T i = value;
    char* p = buff;

    do {
        int j = static_cast<int>(i % 10);
        i /= 10;
        *p++ = ZERO[j];
    } while (i != 0);
    
    if (value < 0)
        *p++ = '-';

    *p = '\0';
    std::reverse(buff, p);

    return p - buff;
}

inline std::string ConvertHexToString(const void* p) {
    std::string answer;
    uintptr_t v = reinterpret_cast<uintptr_t>(p);

    do {
        int j = static_cast<int>(v % 16);
        v /= 16;
        answer = answer + DIGITSHEX[j];

    } while (v != 0);

    std::reverse(answer.begin(), answer.end());

    return "0x" + answer;
}

#ifdef __LINUX__
inline bool ExeShellCommand(const std::string& cmd, std::vector<std::string>& res) {
    FILE *fres = NULL;
    char buffer[1024];

    if ((fres = popen(cmd.c_str(), "r")) != NULL) {
        while (fgets(buffer, sizeof(buffer), fres) != NULL) {
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';
            res.push_back(buffer);
        }
        pclose(fres);
        return true;
    }

    return false;
}
#endif

inline std::vector<std::string> SplitString(const std::string& input, const std::string& separator) {
    std::vector<std::string> res;
    std::string::size_type last = 0;
    std::string::size_type cur = 0;

    while (last <= input.length()) {
        cur = input.find(separator, last);

        if (cur != std::string::npos) {
            res.push_back(input.substr(last, cur - last));
            last = cur + separator.length();
        } else {
            res.push_back(input.substr(last));
            break;
        }
    }

    return res;
}

inline std::string JoinString(const std::vector<std::string>& input, const std::string& separator) {
    std::string res;

    for (size_t i = 0; i < input.size(); i++) {
        res += input[i];
        if (i < input.size() - 1)
            res += separator;
    }

    return res;
}


inline void TrimStringLeft(std::string& input) {
    size_t from = 0;
    while (from < input.length() && isspace(input[from]))
        from++;
    input = input.substr(from);
}

inline void TrimStringRight(std::string& input)
{
    size_t to = input.length() - 1;
    while (to >= 0 && isspace(input[to]))
        to--;
    input = input.substr(0, to + 1);
}

inline void TrimString(std::string& input)
{
    // ' ', '\t', '\r', '\n', '\v', '\f'
    TrimStringLeft(input);
    TrimStringRight(input);
}


//inline const char* trimStringLeft_SIMD(const char* p)
//{
//    const char* nextAligned = reinterpret_cast<const char*>(reinterpret_cast<size_t>(p) + 15 & ~15);
//    while (p != nextAligned)
//    {
//        if (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n' || *p == '\v' || *p == '\f')
//            ++p;
//        else 
//            return p;
//    }
//
//    static const char whitespace[16] = " \t\r\n\v\f";
//    const __m128i w = _mm_loadu_si128(reinterpret_cast<const __m128i *>(&whitespace[0]));
//    
//    std::cout << "hello" << std::endl;
//
//    for (;; p += 16)
//    {
//        const __m128i s = _mm_load_si128(reinterpret_cast<const __m128i *>(p));
//        
//        /* 
//         * _SIDD_UBYTE_OPS          : 操作数unsigned char 
//         * _SIDD_CMP_EQUAL_ANY      : 匹配指定字符中的任何一个
//         * _SIDD_BIT_MASK           : 返回比特结果 
//         * _SIDD_NEGATIVE_POLARITY  : 反转结果，1代表非指定字符
//         *
//         * */
//        const int r = _mm_cvtsi128_si32(
//                _mm_cmpistrm(w, s, 
//                    _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK | _SIDD_NEGATIVE_POLARITY));
//        
//        if (r != 0)
//            return p + __builtin_ffs(r) - 1;
//    }
//}

}

#endif /* __UTILS_H__ */
