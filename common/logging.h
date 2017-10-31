// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//
#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "log_stream.h"
#include "time_wrapper.h"

class Logging {
public:
    enum Level { TRACE, DEBUG, INFO, WARN, ERROR, FATAL, LEVELS_NUM};

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
    };

    Core core_;
};

extern Logging::Level GlobalLevel;

#define LOG_TRACE if (Logging::GetLevel() <= Logging::TRACE) \
    Logging(__FILE__, __LINE__, Logging::TRACE, __func__).Stream()

#define LOG_DEBUG if (Logging::GetLevel() <= Logging::DEBUG) \
    Logging(__FILE__, __LINE__, Logging::DEBUG, __func__).Stream()

#define LOG_INFO if (Logging::GetLevel() <= Logging::INFO) \
    Logging(__FILE__, __LINE__).Stream()

#define LOG_WARN Logging(__FILE__, __LINE__, Logging::WARN).Stream()

#define LOG_ERROR Logging(__FILE__, __LINE__, Logging::ERROR).Stream()

#define LOG_FATAL Logging(__FILE__, __LINE__, Logging::FATAL).Stream()

#define LOG_SYSERR Logging(__FILE__, __LINE__, false).Stream()

#define LOG_SYSFATAL Logging(__FILE__, __LINE__, true).Strea()


#endif /* __LOGGING_H__ */

