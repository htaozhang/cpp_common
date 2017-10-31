
#include <string>

#include "gtest/gtest.h"
#include "mysql_wrapper.h"


TEST(MySqlWrapper, Connect) {
    MySqlWrapper mysql("127.0.0.1", "", "", "test");
    EXPECT_EQ(true, mysql.Connect());
}

TEST(MySqlWrapper, Insert) {
    std::string sql = "insert into user(name, age) values('zhangsan', 50),('lisi', 60);";
    
    MySqlWrapper mysql("127.0.0.1", "", "", "test");
    EXPECT_EQ(true, mysql.Connect());
    //EXPECT_EQ(true, mysql.Insert(sql));
}

TEST(MySqlWrapper, Delete) {
    std::string sql = "delete from user where id = 2;";
    MySqlWrapper mysql("127.0.0.1", "", "", "test");
    EXPECT_EQ(true, mysql.Connect());
    //EXPECT_EQ(true, mysql.Delete(sql));
}

TEST(MySqlWrapper, Update) {
    std::string sql = "update user set age = 100 where id = 1;";
    MySqlWrapper mysql("127.0.0.1", "", "", "test");
    EXPECT_EQ(true, mysql.Connect());
    //EXPECT_EQ(true, mysql.Update(sql));
}

TEST(MySqlWrapper, Query) {
    std::string sql = "select id, name, age from user;";
    std::vector<std::vector<std::string> > ans;
    std::vector<std::vector<std::string> > obj;
    std::vector<std::string> tmp = {"1", "zhangsan", "100"};
    obj.push_back(tmp);

    MySqlWrapper mysql("127.0.0.1", "", "", "test");
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ(true, mysql.Query(sql, ans));
    EXPECT_EQ(ans, obj);
}

TEST(MySqlWrapper, ExcapeString) {  
    std::string field = "my name\\'s xxx";
    MySqlWrapper mysql("127.0.0.1", "", "", "test");
    EXPECT_EQ(true, mysql.Connect());
    EXPECT_EQ(field, mysql.ExcapeString("my name's xxx"));
}




