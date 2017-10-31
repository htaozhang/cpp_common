// Copyright (c) 2017. All rights reserved
#ifndef __STRUCT_A_H__
#define __STRUCT_A_H__

struct Struct_A {
    bool m_bool;
    std::string m_string;
    int32_t m_int;
    uint32_t m_uint;
    double m_double;
    int8_t m_char;
    uint8_t m_uchar;
    int16_t m_short;
    uint16_t m_ushort;
    int64_t m_int64;
    uint64_t m_uint64;
    float m_float;
    std::vector<std::string> m_vector_string;
    std::vector<int32_t> m_vector_int;
    std::map<std::string, std::string> m_map_string;
    std::map<std::string, int> m_map_int;

    Struct_A() 
        : m_bool(true),
          m_string("Struct_A"),
          m_int((1LL<<31) - 1),
          m_uint(0),
          m_double(0.0),
          m_char(-1),
          m_uchar(0),
          m_short(-1),
          m_ushort(0),
          m_int64((1LL<<32) - 1),
          m_uint64(0),
          m_float(0.0)
    {
    }
    bool operator==(const Struct_A &that) const {
        if (m_bool != that.m_bool 
            || m_string != that.m_string
            || m_int != that.m_int 
            || m_uint != that.m_uint 
            || m_double != that.m_double 
            || m_char != that.m_char 
            || m_uchar != that.m_uchar 
            || m_short != that.m_short 
            || m_ushort != that.m_ushort 
            || m_int64 != that.m_int64 
            || m_uint64 != that.m_uint64 
            || m_float != that.m_float
            || m_vector_string != that.m_vector_string 
            || m_vector_int != that.m_vector_int 
            || m_map_string != that.m_map_string
            || m_map_int != that.m_map_int) {
            return false;
        } 

        return true;

    }
};


bool toJsonValue(const Struct_A &input, rapidjson::Value &output, jsonreflect::Alloc &alloc) {
    if (!jsonreflect::toJsonValue(input.m_bool, "m_bool", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_string, "m_string", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_int, "m_int", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_uint, "m_uint", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_double, "m_double", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_char, "m_char", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_uchar, "m_uchar", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_short, "m_short", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_ushort, "m_ushort", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_int64, "m_int64", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_uint64, "m_uint64", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_float, "m_float", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_vector_string, "m_vector_string", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_vector_int, "m_vector_int", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_map_string, "m_map_string", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.m_map_int, "m_map_int", output, alloc)) return false;
    return true;
}

bool jsonValueTo(const rapidjson::Value &input, Struct_A &output) {
    if (!jsonreflect::jsonValueTo(input, "m_bool", output.m_bool)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_string", output.m_string)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_int", output.m_int)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_uint", output.m_uint)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_double", output.m_double)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_char", output.m_char)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_uchar", output.m_uchar)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_short", output.m_short)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_ushort", output.m_ushort)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_int64", output.m_int64)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_uint64", output.m_uint64)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_float", output.m_float)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_vector_string", output.m_vector_string)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_vector_int", output.m_vector_int)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_map_string", output.m_map_string)) return false;
    if (!jsonreflect::jsonValueTo(input, "m_map_int", output.m_map_int)) return false;
    return true;
}


#endif /* __STRUCT_A_H__ */
