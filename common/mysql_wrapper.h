// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __MYSQL_WRAPPER_H__
#define __MYSQL_WRAPPER_H__

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <mysql.h>
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <mysql/mysql.h>
#endif

#include <cstdlib>
#include <string>
#include <vector>


class MySqlWrapper
{
    typedef std::vector<std::vector<std::string> > RESULT;
    
public:
    MySqlWrapper() {}
    MySqlWrapper(const std::string& host, 
            const std::string& user, 
            const std::string& passwd, 
            const std::string& db,
            unsigned port);
    ~MySqlWrapper();

    bool Connect();
    bool Disconnect();
    void Reset(const std::string&,
               const std::string&,
               const std::string&,
               const std::string&,
               unsigned);
    std::string ExcapeString(const std::string& field, char quote = '\'');
    bool Insert(const std::string& sql);
    bool Delete(const std::string& sql);
    bool Update(const std::string& sql);
    bool Query(const std::string& sql);
    bool Query(const std::string& sql, RESULT& output);
    bool UnbufferedQuery(const std::string& sql, RESULT& output);
    
    int Errno();
    std::string Error();

private:
    bool ExecuteSQL(const std::string& sql);

private:
    MYSQL       mysql_;
    bool        connected_;
    std::string host_;
    std::string user_;
    std::string passwd_;
    std::string db_;
    unsigned    port_;
};

#endif /* __MYSQL_WRAPPER_H__ */

