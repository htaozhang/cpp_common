#include "logging.h"

int main() {
    {
        LOG_TRACE << "trace";
        LOG_DEBUG << "debug";
        LOG_INFO << "Hello";
        LOG_INFO << "World";
    }
    return 0;
}
