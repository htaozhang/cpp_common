// Copyright(c) 2017 macrozhang
// All rights reserved.
//

#include <iostream>

#include "mysql_wrapper.h"

#include <assert.h>
#include <string.h>

using namespace std;

MySqlWrapper::MySqlWrapper(const string& host, 
        const string& user, 
        const string& passwd, 
        const string& db) : host_(host), user_(user), passwd_(passwd), db_(db), connected_(false) {
    mysql_ = (MYSQL*)malloc(sizeof(MYSQL));
    mysql_init(mysql_);
}

MySqlWrapper::~MySqlWrapper() {
    if (connected_) {
        mysql_close(mysql_);
        if (mysql_) 
            free(mysql_);
    }
}


bool MySqlWrapper::Connect() {
    if (!mysql_real_connect(mysql_, 
                host_.c_str(), 
                user_.c_str(), 
                passwd_.c_str(), 
                (db_.empty() ? NULL : db_.c_str()), 
                0, 
                NULL, 
                0)) {
        mysql_close(mysql_);
        return false;
    }

    mysql_set_character_set(mysql_, "utf8");
    connected_ = true;
    return true;
}

bool MySqlWrapper::Disconnect() {
    connected_ = false;
    mysql_close(mysql_);
    return true;
}

string MySqlWrapper::ExcapeString(const string& field) {
    string answer;
    size_t len = 2 * (field.length()) + 1;
    char* tmp = (char*)malloc(len);
    // mysql version: >5.6
    // mysql_real_escape_string_quote(mysql_, tmp, field.c_str(), field.length(), '\'');
    unsigned long n = 
        mysql_real_escape_string(mysql_, tmp, field.c_str(), field.length());

    assert(n < len);

    tmp[n] = '\0';
    answer = tmp;
    free(tmp);
    return answer;
}


bool MySqlWrapper::Insert(const string& sql) {
    return ExecuteSQL(sql);
}

bool MySqlWrapper::Delete(const string& sql) {
    return ExecuteSQL(sql);
}

bool MySqlWrapper::Update(const string& sql) {
    return ExecuteSQL(sql);
}

bool MySqlWrapper::Query(const string& sql, vector<vector<string> >& output) {
    if (!ExecuteSQL(sql))
        return false;

    MYSQL_RES* res = mysql_store_result(mysql_);   
    if (res) {
        unsigned long size = mysql_num_fields(res);
        while (MYSQL_ROW row = mysql_fetch_row(res)) {
            std::vector<std::string> tmp;
            tmp.reserve(size);
            for (unsigned long i = 0; i < size; i++)
                tmp.push_back(row[i] ? row[i] : "");
            output.push_back(tmp);
        }

        mysql_free_result(res);
    }

    return true;
}


bool MySqlWrapper::ExecuteSQL(const string& sql) {
    if (!connected_ || 
            mysql_real_query(mysql_, sql.c_str(), strlen(sql.c_str()))) {
        return false;
    }

    return true;
}

int MySqlWrapper::Errno() {
    return mysql_errno(mysql_);
}


string MySqlWrapper::Error() {
    return mysql_error(mysql_);
}


