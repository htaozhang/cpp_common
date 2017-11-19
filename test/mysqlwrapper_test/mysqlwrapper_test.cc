
#include <string>


#include "mysql_wrapper.h"
#include "gtest/gtest.h"

#define HOST "127.0.0.1"
#define USER "admin"
#define PASSWD "12345678"
#define DB "test"

TEST(MySqlWrapper, Connect) {
    MySqlWrapper mysql(HOST, USER, PASSWD, DB);
    EXPECT_EQ(true, mysql.Connect());
    std::string sql = "drop table if exists mysql_wrapper_unittest;";
    EXPECT_EQ(true, mysql.Query(sql));
    sql = "create table mysql_wrapper_unittest(id int auto_increment primary key,name varchar(32), age int);";
    EXPECT_EQ(true, mysql.Query(sql));
}

TEST(MySqlWrapper, Insert) {
    std::string sql = "insert into mysql_wrapper_unittest(name, age) values('zhangsan', 50),('lisi', 60);";
    
    MySqlWrapper mysql(HOST, USER, PASSWD, DB);
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ(true, mysql.Insert(sql));
}

TEST(MySqlWrapper, Delete) {
    std::string sql = "delete from mysql_wrapper_unittest where name = 'zhangsan';";
    MySqlWrapper mysql(HOST, USER, PASSWD, DB);
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ(true, mysql.Delete(sql));
}

TEST(MySqlWrapper, Update) {
    std::string sql = "update mysql_wrapper_unittest set age = 100 where name = 'lisi';";
    MySqlWrapper mysql(HOST, USER, PASSWD, DB);
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ(true, mysql.Update(sql));
}

TEST(MySqlWrapper, Query) {
    std::string sql = "select name, age from mysql_wrapper_unittest;";
    std::vector<std::vector<std::string> > ans;
    std::vector<std::vector<std::string> > obj;
    std::vector<std::string> tmp = {"lisi", "100"};
    obj.push_back(tmp);

    MySqlWrapper mysql(HOST, USER, PASSWD, DB);
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ(true, mysql.Query(sql, ans));
    EXPECT_EQ(ans, obj);
}

TEST(MySqlWrapper, ExcapeString) {
    MySqlWrapper mysql(HOST, USER, PASSWD, DB);
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ("my name\\'s xxx\\\"", mysql.ExcapeString("my name's xxx\"", '\''));
    EXPECT_EQ("my name\\'s xxx\\\"", mysql.ExcapeString("my name's xxx\"", '"'));
}

