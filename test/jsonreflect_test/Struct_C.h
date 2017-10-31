// Copyright (c) 2017. All rights reserved
#ifndef __STRUCT_C_H__
#define __STRUCT_C_H__

#include "Struct_B.h"

struct Struct_C {
    bool flag_;
    int id_;
    std::string data_;
    Struct_B struct_b_;

    Struct_C()
        : flag_(false),
          id_(0),
          data_("") {
    }

    bool operator==(const Struct_C &that) const {
        if (flag_ != that.flag_ || id_ != that.id_ || data_ != that.data_ || !(struct_b_ == that.struct_b_))
            return false;
        return true;
    }
};


bool toJsonValue(const Struct_C &input, rapidjson::Value &output, jsonreflect::Alloc &alloc) {
    if (!jsonreflect::toJsonValue(input.flag_, "flag_", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.id_, "id_", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.data_, "data_", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.struct_b_, "struct_b_", output, alloc)) return false;
    return true;
}

bool jsonValueTo(const rapidjson::Value &input, Struct_C &output) {
    if (!jsonreflect::jsonValueTo(input, "flag_", output.flag_)) return false;
    if (!jsonreflect::jsonValueTo(input, "id_", output.id_)) return false;
    if (!jsonreflect::jsonValueTo(input, "data_", output.data_)) return false;
    if (!jsonreflect::jsonValueTo(input, "struct_b_", output.struct_b_)) return false;
    return true;
}



#endif /* __STRUCT_C_H__ */
