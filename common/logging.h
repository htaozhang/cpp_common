// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "log_stream.h"
#include "time_wrapper.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #define __thread __declspec(thread)
    #define strerror_r(errno, buf, len) strerror_s(buf, len, errno)
#endif

// lumbda expression
static const bool HAS_COLOR = [](){
#if defined(_WIN32)
    return false;
#else
    if (const char* term = std::getenv("TERM")) {
        return (strncmp(term, "linux", 5) == 0 ||
                strncmp(term, "xterm", 5) == 0);
    } else {
        return false;
    }
#endif
}();

class Logging {
public:
    enum Level {
        L_TRACE,
        L_DEBUG,
        L_INFO,
        L_WARN,
        L_ERROR,
        L_FATAL,
        LEVELS_NUM
    };

    class SourceFile {
    public:
        template<int N>
        inline SourceFile(const char (&arr)[N])
            : data_(arr),                
              length_(N - 1) { 
            if (const char* slash = strrchr(data_, '/')) {
                data_ = slash + 1;       
                length_ -= static_cast<int>(data_ - arr);
            }
        }                                
        
        explicit SourceFile(const char*);
        const char* data_;
        int length_;
    };

    Logging(SourceFile, int);
    Logging(SourceFile, int, Level);
    Logging(SourceFile, int, Level, const char*);
    Logging(SourceFile, int, bool);
    ~Logging();

    LogStream& Stream();

    static Level GetLevel();
    static void SetLevel(Level);

    typedef void (*FlushFunc)();
    typedef void (*OutputFunc)(const char*, int);
    static void SetOutput(OutputFunc);
    static void SetFlush(FlushFunc);

private:
    class Core {
    public:
        Core(Level, SourceFile, int, int);
        void Over();

        TimeWrapper time_;
        Level level_;
        SourceFile file_;
        int line_;
        LogStream stream_;
        
        // terminal color
#if defined(_WIN32) || defined(__CYGWIN__)
#define COLOR(ID) ("\x1b[1;" #ID "m")
#else
#define COLOR(ID) ("\e[" #ID "m")
#endif
        inline const char* ColorReset()    { return HAS_COLOR ? COLOR(0) : ""; }
        inline const char* ColorRed()      { return HAS_COLOR ? COLOR(31) : ""; }
        inline const char* ColorYellow()   { return HAS_COLOR ? COLOR(33) : ""; }
    };
    
    Core core_;
};

extern Logging::Level GlobalLevel;

#define LOG_TRACE if (Logging::GetLevel() <= Logging::L_TRACE) \
    Logging(__FILE__, __LINE__, Logging::L_TRACE, __func__).Stream()

#define LOG_DEBUG if (Logging::GetLevel() <= Logging::L_DEBUG) \
    Logging(__FILE__, __LINE__, Logging::L_DEBUG, __func__).Stream()

#define LOG_INFO if (Logging::GetLevel() <= Logging::L_INFO) \
    Logging(__FILE__, __LINE__).Stream()

#define LOG_WARN Logging(__FILE__, __LINE__, Logging::L_WARN).Stream()

#define LOG_ERROR Logging(__FILE__, __LINE__, Logging::L_ERROR).Stream()

#define LOG_FATAL Logging(__FILE__, __LINE__, Logging::L_FATAL).Stream()

#define LOG_SYSERR Logging(__FILE__, __LINE__, false).Stream()

#define LOG_SYSFATAL Logging(__FILE__, __LINE__, true).Stream()


#endif /* __LOGGING_H__ */

