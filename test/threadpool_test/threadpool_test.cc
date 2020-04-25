#include "thread_pool.h"

#include "unistd.h"
#include <iostream>

int main(int argc, char *argv[]) {
    {
    ThreadPool pool;
    pool.SetMaxQueueSize(2);
    pool.Start(2);
    pool.Run([](){ std::cout << "hello" << std::endl; });
    pool.Run([](){ std::cout << "world" << std::endl; });
    pool.Run([](){ std::cout << "hi" << std::endl; });
    usleep(100);
    pool.Stop();
    }
    return 0;
}