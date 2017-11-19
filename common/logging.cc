// Copyright(c)  macrozhang  
// All rights reserved.          
//                               
//                               
#include <utility>  // pair

#include <stdio.h>  // fflush, fwrite
#include <string.h> // strlen, strerror_r/strerror_s 
#include <stdlib.h> // abort
#include <assert.h>
#include <errno.h>  // errno

#include "logging.h"   
#include "generic_time.h"   // gmtime
#include "generic_thread.h" // get_thread_name

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
Logging::Level GlobalLevel = Logging::L_INFO;

const char* LevelName[Logging::LEVELS_NUM] 
    = { "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL " };


///
/// class Logging::SourceFile
///
Logging::SourceFile::SourceFile(const char* filename) 
    : data_(filename)
{
    if (const char* slash = strrchr(filename, '/'))
        data_ = slash + 1;       

    length_ = static_cast<int>(strlen(data_));
}                                


///
/// class Logging
///
Logging::Logging(SourceFile file, int line) 
    : core_(L_INFO, file, line, 0)
{

}                      

Logging::Logging(SourceFile file, int line, Level level)
    : core_(level, file, line, 0)
{

}                      

Logging::Logging(SourceFile file, int line, Level level, const char* func)
    : core_(level, file, line, 0)
{
    core_.stream_ << func << ' ';
}

Logging::Logging(SourceFile file, int line, bool abort) 
    : core_(abort ? L_FATAL : L_ERROR, file, line, errno) {

}                                               

Logging::~Logging() {                           
    core_.Over();
    GlobalOutput(Stream().Cache().Data(), Stream().Cache().Length());
    if (core_.level_ == L_FATAL) {
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
      line_(line)
{
    // color
    if (level == Level::L_WARN) {
        stream_ << ColorReset() << ColorYellow();
    }
    else if (level_ >= Level::L_ERROR) {
        stream_ << ColorReset() << ColorRed();
    }
    
    // time
    time_t seconds = time_.SecondsSinceEpoch();
    int microseconds = static_cast<int>(
                                        time_.MicroSecondsSinceEpoch() %
                                        TimeWrapper::microSecondsPerSecond);
    
    if (_tseconds != seconds) {
        _tseconds = seconds;
        std::tm tmtime = gmtime(seconds);
        int len = snprintf(_ttime,
                           sizeof(_ttime),
                           "%4d%02d%02d %02d:%02d:%02d",
                           tmtime.tm_year + 1900,
                           tmtime.tm_mon + 1,
                           tmtime.tm_mday,
                           tmtime.tm_hour,
                           tmtime.tm_min,
                           tmtime.tm_sec);
        
        assert(len == 17);
        
        (void)len;
    }
          
    stream_ << PAIR(_ttime, 17) << Format(".%06dZ ", microseconds);
    
    // thread name/id
    stream_ << get_thread_name() << " ";
    
    // level
    stream_ << PAIR(LevelName[level], 6);

    // error msg
    if (errnum != 0) {
        stream_ << strerror_r(errnum, _terror, sizeof(_terror)) << " (errno=" << errnum << ") ";
    }
}

void Logging::Core::Over() {
    stream_ << " - " << PAIR(file_.data_, file_.length_) << ":" << line_;
    if (level_ >= Level::L_WARN)
        stream_ << ColorReset();
    stream_ << "\n";
}   

