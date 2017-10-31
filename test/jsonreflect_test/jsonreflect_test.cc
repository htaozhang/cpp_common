#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <string>

#include "json_reflect.h"
#include "Struct_A.h"
#include "Struct_B.h"
#include "Struct_C.h"

#ifdef WIN32
    #pragma warning(disable: 4005)
    #pragma warning(disable: 4244)
    #include <time.h>
    #include <Windows.h>
    #define INT64_MIN  LLONG_MIN
    #define INT64_MAX  LLONG_MAX
    #define UINT64_MAX ULLONG_MAX
#elif __APPLE__
    #include <sys/time.h>
    typedef suseconds_t __suseconds_t;
#else
    #include <sys/time.h>
    #ifndef INT64_MIN
        #define INT64_MIN   LONG_LONG_MIN
    #endif
    #ifndef INT64_MAX
        #define INT64_MAX   LONG_LONG_MAX
    #endif
    #ifndef UINT64_MAX
        #define UINT64_MAX  ULONG_LONG_MAX
    #endif
#endif

using namespace std;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
        test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_TRUE(expect, actual) EXPECT_EQ_BASE((actual) != false, "true", "false", "%s")
#define EXPECT_EQ_FALSE(expect, actual) EXPECT_EQ_BASE((actual) == false, "false", "true", "%s")
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_INT64(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%" PRId64)
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
#define EXPECT_EQ_STRING(expect, actual, alength) \
    EXPECT_EQ_BASE(strlen(expect) == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")


template<typename T>
void test_base(const T &input) {
    T ret;
    rapidjson::Value output;
    rapidjson::Document doc;
    if (!jsonreflect::toJsonValue(input, output, doc.GetAllocator())) {
        EXPECT_EQ_TRUE(true, false);
        return;
    }

    if (!jsonreflect::jsonValueTo(output, ret)) {
        EXPECT_EQ_TRUE(true, false);
        return;
    }
        
    EXPECT_EQ_TRUE(true, ret == input);
}

template<typename T>
void test_base(const T &input, const char *name) {
    rapidjson::Value output;
    rapidjson::Document doc;

    if (!jsonreflect::toJsonValue(input, name, output, doc.GetAllocator())) {
        EXPECT_EQ_TRUE(true, false);
        return;
    }

    T ret;
    if (!jsonreflect::jsonValueTo(output, name, ret)) {
        EXPECT_EQ_TRUE(true, false);
        return;
    }

    EXPECT_EQ_TRUE(true, ret == input);
}

template<typename T>
void test_item(const T &input) {
    std::string output;
    if (!jsonreflect::serialize(input, output)) {
        EXPECT_EQ_TRUE(true, false);
        return;
    }
    std::cout << "serialize:" << output << std::endl;
    T ret;
    if (!jsonreflect::deserialize(output, ret)) {
        EXPECT_EQ_TRUE(true, false);
        return;
    }
    EXPECT_EQ_TRUE(true, ret == input);
}

void test_base_2() {
    bool bool_val = true;
    test_base<bool>(bool_val, "bool_val");
    int8_t int8_t_val = SCHAR_MIN;
    test_base<int8_t>(int8_t_val, "int8_t_val");
    uint8_t uint8_t_val = UCHAR_MAX;
    test_base<uint8_t>(uint8_t_val, "uint8_t_val");
    int16_t int16_t_val = SHRT_MIN;
    test_base<int16_t>(int16_t_val, "int16_t_val");
    uint16_t uint16_t_val = USHRT_MAX;
    test_base<uint16_t>(uint16_t_val, "uint16_t_val");
    int32_t int32_t_val = INT_MIN;
    test_base<int32_t>(int32_t_val, "int32_t_val");
    uint32_t uint32_t_val = UINT_MAX;
    test_base<uint32_t>(uint32_t_val, "uint32_t_val");
    int64_t int64_t_val = INT64_MIN;
    test_base<int64_t>(int64_t_val, "int64_t_val");
    uint64_t uint64_t_val = UINT64_MAX;
    test_base<uint64_t>(uint64_t_val, "uint64_t_val");
    float float_val = FLT_MIN_EXP;
    test_base<float>(float_val, "float_val");
    double double_val = DBL_MIN_EXP;
    test_base<double>(double_val, "double_val");
    std::string string_val = "hello";
    test_base<std::string>(string_val, "string_val");
    test_base<std::vector<int> >(std::vector<int>(), "vec_int_val");
    std::vector<int> vec_int_val{5, 2, 3, 4};
    test_base<std::vector<int> >(vec_int_val, "vec_int_val");

    test_base<std::vector<std::string> >(std::vector<std::string>(), "vec_string_val");
    std::vector<std::string> vec_string_val{"hello", "world"};
    test_base<std::vector<std::string> >(vec_string_val, "vec_string_val");

    std::map<std::string, int> map_int_val;
    test_base<std::map<std::string, int> >(map_int_val, "map_int_val");
    map_int_val["hello"] = 1;
    map_int_val["world"] = 2;
    test_base<std::map<std::string, int> >(map_int_val, "map_int_val");


    std::map<std::string, std::string> map_string_val;
    test_base<std::map<std::string, std::string> >(map_string_val, "map_string_val");
    map_string_val["hello"] = "hello";
    map_string_val["world"] = "world";
    test_base<std::map<std::string, std::string> >(map_string_val, "map_string_val");
}

void test_base_1() {
    test_base<bool>(true);
    test_base<bool>(false);
    test_base<int8_t>(SCHAR_MIN);
    test_base<int8_t>(SCHAR_MAX);
    test_base<uint8_t>(UCHAR_MAX);
    test_base<int16_t>(SHRT_MIN);
    test_base<int16_t>(SHRT_MAX);
    test_base<uint16_t>(USHRT_MAX);
    test_base<int32_t>(INT_MIN);
    test_base<int32_t>(INT_MAX);
    test_base<uint32_t>(UINT_MAX);
    test_base<int64_t>(INT64_MIN);
    test_base<int64_t>(INT64_MAX);
    test_base<uint64_t>(UINT64_MAX);
    test_base<float>(FLT_MIN_EXP);
    test_base<float>(FLT_MAX_EXP);
    test_base<double>(DBL_MIN_EXP);
    test_base<double>(DBL_MAX_EXP);
    test_base<std::string>(std::string("hello"));

    test_base<std::vector<int> >(std::vector<int>());
    std::vector<int> vec_int{1, 2, 3, 4};
    test_base<std::vector<int> >(vec_int);

    test_base<std::vector<std::string> >(std::vector<std::string>());
    std::vector<std::string> vec_string{"hello", "world"};
    test_base<std::vector<std::string> >(vec_string);

    std::map<std::string, int> map_int;
    test_base<std::map<std::string, int> >(map_int);
    map_int["hello"] = 1;
    map_int["world"] = 2;
    test_base<std::map<std::string, int> >(map_int);


    std::map<std::string, std::string> map_string;
    test_base<std::map<std::string, std::string> >(map_string);
    map_string["hello"] = "hello";
    map_string["world"] = "world";
    test_base<std::map<std::string, std::string> >(map_string);
  
    test_item<bool>(true);
    test_item<std::vector<int> >(vec_int);
    test_item<std::vector<std::string> >(vec_string);
    test_item<std::map<std::string, int> >(map_int);
    test_item<std::map<std::string, std::string> >(map_string);
}

void test_struct() {
    Struct_A struct_a;
    test_item<Struct_A>(struct_a);
    Struct_B struct_b;
    test_item<Struct_B>(struct_b);
    Struct_C struct_c;
    test_item<Struct_C>(struct_c);
}

int main() {
	test_base_2();
    test_base_1();
    test_struct();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}


