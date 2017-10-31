// Copyright(c) 2017 macrozhang.
// All rights reserved.
//

#include "redis_wrapper.h"

#include <iostream>

using namespace std;

#define PREPARE_WORKING \
    do { \
        if (!redis_) { \
            return false; \
        } \
        redis_->err = 0; \
    } while(0); 

RedisWrapper::RedisWrapper() : redis_(NULL) {

}

RedisWrapper::~RedisWrapper() {
    if (redis_) {
        redisFree(redis_);
        redis_ = NULL;
    }
}

bool RedisWrapper::Init(const std::string& addr, const timeval& timeout, int db/* = 0 */) {
    size_t pos;
    if ((pos = addr.find(":")) == std::string::npos) {
        return false;
    }

    ip_ = addr.substr(0, pos);
    port_ = atoi(addr.substr(pos+1).c_str());
    db_ = db;
    timeout_ = timeout;

    return Connect();
}

redisContext* RedisWrapper::SelectDB(redisContext *c) {
    redisReply *reply = (redisReply*)redisCommand(c,"SELECT %d", db_);
    if (reply != NULL) {
        freeReplyObject(reply);
        reply = NULL;
    }
    return c;
}

void RedisWrapper::Disconnect(redisReply *r/* = NULL */) {
    if (redis_) {
        redisFree(redis_);
        redis_ = NULL;
    }
    if (r) {
        freeReplyObject(r);
        r = NULL;
    }
}

bool RedisWrapper::Connect() {
    redis_ = redisConnectWithTimeout(ip_.c_str(), port_, timeout_);
    redis_ = SelectDB(redis_);
    if (redis_ == NULL || HasError()) {
        Disconnect();
        return false;
    }
    
    return true;
}


redisReply* RedisWrapper::DoCommand(const char* cmd) {
    if (redis_)
        return (redisReply*)redisCommand(redis_, cmd);
    else
        return NULL;
}

bool RedisWrapper::Set(const std::string& key, const std::string& val, int timeout/* = 0 */) {
    PREPARE_WORKING;

    redisReply *r = NULL;
    if (timeout > 0) {
        r = (redisReply*)redisCommand(redis_, "SET %s %s ex %d", key.c_str(), val.c_str(), timeout);
    } else {
        r = (redisReply*)redisCommand(redis_, "SET %s %s", key.c_str(), val.c_str());
    }
   
    if (HasError() ||
        r == NULL || 
        r->type != REDIS_REPLY_STATUS ||
        strcmp(r->str, "OK")) {
        Disconnect(r);
        return false;
    }

    Disconnect(r);
    return true;
}

bool RedisWrapper::Mget(const std::vector<std::string>& keys, std::vector<std::string>& vals) {
    PREPARE_WORKING;

    vector<const char*> argv(keys.size() + 1);
    vector<size_t> argvlen(keys.size() + 1);
    for (size_t i = 0; i < argv.size(); i++) {
        argv[i] = (i == 0 ? "MGET" : keys[i-1].c_str());
        argvlen[i] = (i == 0 ? strlen("MGET") : keys[i-1].length());
    }

    redisReply* r = (redisReply*)redisCommandArgv(redis_, (int)argv.size(), &(argv[0]), &(argvlen[0]));
    if (HasError() ||
        r == NULL ||
        r->type != REDIS_REPLY_ARRAY) {
        Disconnect(r);
        return false;
    }

    for (size_t i = 0; i < r->elements; i++) {
        redisReply* childReply = r->element[i];
        if (childReply && 
            childReply->type == REDIS_REPLY_STRING) {
            vals.push_back(childReply->str);
        } else if (childReply && 
            childReply->type == REDIS_REPLY_NIL) {
            vals.push_back("");
        }
    }

    if (vals.size() != keys.size()) {
        vals.clear();
        cerr << "[ERROR][RedisWrapper::mget keys.size not match vals.size]" << endl;
    }

    Disconnect(r);
    return true;
}

bool RedisWrapper::Keys(const std::string& pattern, std::vector<std::string>& keys) {
    return true;
}

bool RedisWrapper::Hkeys(const std::string& pattern, std::vector<std::string>& keys) {
    return true;
}

bool RedisWrapper::Get(const std::string& key, std::string& val) {
    PREPARE_WORKING;

    redisReply* r = (redisReply*)redisCommand(redis_, "get %s", key.c_str());
    if (HasError() ||
        r == NULL ||
        r->type != REDIS_REPLY_STRING) {
        Disconnect(r);
        return false;
    }

    val = r->str;
    Disconnect(r);
    return true;
}

bool RedisWrapper::Hget(const std::string& key, const std::string& field, std::string& val) {
    PREPARE_WORKING;

    redisReply* r = (redisReply*)redisCommand(redis_, "HGET %s %s", key.c_str(), field.c_str());
    if (HasError() ||
        r == NULL ||
        r->type != REDIS_REPLY_STRING) {
        Disconnect(r);
        return false;
    }    

    val = r->str;
    Disconnect(r);
    return true;
}




