#include "file_lock.h"
#include <gtest/gtest.h>

TEST(CreateTest, create) {

    FileLock fl;
    EXPECT_EQ(0, fl.Create());
    EXPECT_EQ(0, fl.TryLock());
    EXPECT_EQ(0, fl.UnLock());
}

TEST(LockTest, NotUnlock) {
    FileLock fl;
    EXPECT_EQ(0, fl.Create());
    EXPECT_EQ(0, fl.TryLock());
}

/*
TEST(LockTest, Lock) {
    FileLock fl;
    EXPECT_EQ(0, fl.Create());
    EXPECT_EQ(0, fl.TryLock());
    pid_t fpid=fork();   
    if (fpid < 0) {
        printf("error in fork!");   
    } else if (fpid == 0) {  
        FileLock f2;
        EXPECT_EQ(0, f2.Create());
        EXPECT_NE(0, f2.TryLock());
    }
    waitpid(fpid,NULL,0);
}*/


