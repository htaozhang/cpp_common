// Copyright(c) 2017 macrozhang.
// All rights reserved.
//

#ifndef __REDIS_WRAPPER_H__
#define __REDIS_WRAPPER_H__

#include <vector>

#include <string>
#include <cstring>
#include <cstdlib>

#include "hiredis/hiredis.h"


class RedisWrapper {
public:
    RedisWrapper();
    ~RedisWrapper();
    bool Init(const std::string& addr, const timeval& timeout, int db = 0);

    /* string */
    bool Keys(const std::string& pattern, std::vector<std::string>& keys);      
    bool Set(const std::string& key, const std::string& val, int timeout = 0);
    bool Get(const std::string& key, std::string& val);
    bool Mget(const std::vector<std::string>& keys, std::vector<std::string>& vals);        

    /* hash */
    bool Hkeys(const std::string& pattern, std::vector<std::string>& keys);     
    bool Hget(const std::string& key, const std::string& field, std::string& val);  

private:
    inline bool HasError() {
        if (redis_ && redis_->err)
            return true;
        return false;
    }

    inline int Errno() {
        if (redis_)
            return redis_->err;
        return 0;
    }

    inline const char* Error() {
        if (redis_ && redis_->err)
            return redis_->errstr;
        return "";
    }

    inline bool IsConnected() {
        return redis_ != NULL;
    }

    redisReply* DoCommand(const char* key);
    redisContext* SelectDB(redisContext *c);
    bool Connect();
    void Disconnect(redisReply* r = NULL);

private:
    redisContext* redis_;
    std::string ip_;
    int port_;
    int db_;
    timeval timeout_;
};

#endif /* __REDIS_WRAPPER_H__ */
