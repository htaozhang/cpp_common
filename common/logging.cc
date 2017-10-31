// Copyright(c)  macrozhang  
// All rights reserved.          
//                               
//                               

#include "logging.h"             

#include <stdio.h>  // fflush, fwrite
#include <string.h> // strlen, strerror_r 
#include <stdlib.h> // abort
#include <time.h>   // gmtime_r
#include <assert.h>
#include <error.h>  // errno

#include <utility>  // pair

typedef std::pair<const char*, int> PAIR;

///
/// global & thread var
///
__thread char _terror[512];
__thread char _ttime[32];
__thread time_t _tseconds;

void DefaultOutput(const char* msg, int len) {
    size_t n = fwrite(msg, 1, len, stdout);
    (void)n;
}

void DefaultFlush() {
    fflush(stdout);
}

Logging::OutputFunc GlobalOutput = DefaultOutput;
Logging::FlushFunc GlobalFlush = DefaultFlush;
Logging::Level GlobalLevel = Logging::INFO;

const char* LevelName[Logging::LEVELS_NUM] 
    = { "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL " };


///
/// class Logging::SourceFile
///
Logging::SourceFile::SourceFile(const char* filename) 
    : data_(filename) { 
    if (const char* slash = strrchr(filename, '/'))
        data_ = slash + 1;       

    length_ = static_cast<int>(strlen(data_));
}                                


///
/// class Logging
///
Logging::Logging(SourceFile file, int line) 
    : core_(INFO, file, line, 0) {

}                      

Logging::Logging(SourceFile file, int line, Level level)
    : core_(level, file, line, 0) {

}                      

Logging::Logging(SourceFile file, int line, Level level, const char* func)
    : core_(level, file, line, 0) {
    core_.stream_ << func << ' ';
}

Logging::Logging(SourceFile file, int line, bool abort) 
    : core_(abort ? FATAL : ERROR, file, line, errno) {

}                                               

Logging::~Logging() {                           
    core_.Over();
    GlobalOutput(Stream().Cache().Data(), Stream().Cache().Length());
    if (core_.level_ == FATAL) {
        GlobalFlush();
        abort();
    }
}        

LogStream& Logging::Stream() {
    return core_.stream_;
}  

Logging::Level Logging::GetLevel() {
    return GlobalLevel;
}

void Logging::SetLevel(Level level) {
    GlobalLevel = level;
}  

void Logging::SetOutput(OutputFunc func) {
    GlobalOutput = func;
}

void Logging::SetFlush(FlushFunc func) {
    GlobalFlush = func;
}


///
/// class Logging::Core 
///
Logging::Core::Core(Level level, SourceFile file, int line, int errnum) 
    : time_(TimeWrapper::Now()),
      level_(level),
      file_(file),
      line_(line) {
    // time 
    time_t seconds = time_.SecondsSinceEpoch();
    int microseconds = static_cast<int>(time_.MicroSecondsSinceEpoch() % TimeWrapper::microSecondsPerSecond);
    
    if (_tseconds != seconds) {
        _tseconds = seconds;
        struct tm tmtime;

        ::gmtime_r(&seconds, &tmtime);

        int len = snprintf(_ttime, sizeof(_ttime), "%4d%02d%02d %02d:%02d:%02d",
            tmtime.tm_year + 1900, tmtime.tm_mon + 1, tmtime.tm_mday, tmtime.tm_hour, tmtime.tm_min, tmtime.tm_sec);
        assert(len == 17); (void)len;
    }

    stream_ << PAIR(_ttime, 17) << Format(".%06dZ ", microseconds);

    // level
    stream_ << PAIR(LevelName[level], 6);

    // error msg
    if (errnum != 0) {
        stream_ << strerror_r(errnum, _terror, sizeof _terror) << " (errno=" << errnum << ") ";
    }
}

void Logging::Core::Over() {
    stream_ << " - " << PAIR(file_.data_, file_.length_) << ":" << line_ << "\n";
}   

