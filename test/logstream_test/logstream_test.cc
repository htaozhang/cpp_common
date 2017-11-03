#include "gtest/gtest.h"
#include "log_stream.h"

#include <limits.h>

#include <string>

TEST(Buffer, Append) {
    Buffer<> buffer;
    buffer.Append("(null)", 6);
    EXPECT_STREQ("(null)", buffer.ToString().c_str());
}

TEST(Buffer, Length) {
    Buffer<> buffer;
    buffer.Append("(null)", 6);
    EXPECT_EQ(6, buffer.Length());
}

TEST(Buffer, Usable) {
    Buffer<> buffer;
    EXPECT_EQ(4096, buffer.Usable());
    buffer.Append("(null)", 6);
    EXPECT_EQ(4090, buffer.Usable());
}

TEST(LogStream, Operator) {
    LogStream ls;
    ls << true << " ";
    ls << '0' << " ";
    ls << SHRT_MIN << " ";      // -32768
    ls << SHRT_MAX << " ";      //  32767
    ls << USHRT_MAX << " ";     //  65535
    ls << INT_MIN << " ";       // -2147483648
    ls << INT_MAX << " ";       //  2147483647
    ls << UINT_MAX << " ";      //  4294967295
    //ls << LONG_MIN << " ";      // -2147483648
    //ls << LONG_MAX << " ";      //  2147483647
    //ls << ULONG_MAX << " ";     //  4294967295
    ls << LLONG_MIN << " ";     // -9223372036854775808
    ls << LLONG_MAX << " ";     //  9223372036854775807
    ls << ULLONG_MAX << " ";    //  18446744073709551615 
    ls << 3.14 << " ";
    ls << 3.141592653589793238462643383279 << " ";
    ls << "hello ";
    ls << std::string("world") << " ";
    ls << Format("%d", 1234567890) << " ";

    Buffer<> buffer;
    buffer.Append("<<buffer", 8);
    ls << buffer;

    EXPECT_STREQ("1 0 -32768 32767 65535 -2147483648 2147483647 4294967295 -9223372036854775808 9223372036854775807 18446744073709551615 3.14 3.14159265359 hello world 1234567890 <<buffer",
        ls.Cache().ToString().c_str());
}



