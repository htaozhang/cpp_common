// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __MYSQL_WRAPPER_H__
#define __MYSQL_WRAPPER_H__

#include <mysql/mysql.h>

#include <string>
#include <vector>
#include <cstdlib>

using namespace std;


class MySqlWrapper
{
public:
    MySqlWrapper(const string& host, 
            const string& user, 
            const string& passwd, 
            const string& db);
    ~MySqlWrapper();

    bool Connect();
    bool Disconnect();
    string ExcapeString(const string& field);
    bool Insert(const string& sql);
    bool Delete(const string& sql);
    bool Update(const string& sql);
    bool Query(const string& sql, vector<vector<string> >& output);
    int Errno(); 
    string Error();

private:
    bool ExecuteSQL(const string& sql);

private:
    MYSQL* mysql_;
    string host_;
    string user_;
    string passwd_;
    string db_;
    bool connected_;
};

#endif /* __MYSQL_WRAPPER_H__ */

