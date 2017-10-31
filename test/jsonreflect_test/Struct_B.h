// Copyright (c) 2017. All rights reserved
#ifndef __STRUCT_B_H__
#define __STRUCT_B_H__

#include "Struct_A.h"

struct Struct_B {
    bool flag_;
    int id_;
    std::string data_;
    Struct_A struct_a_;
    Struct_B()
        : flag_(false),
          id_(0),
          data_("") {
    }

    bool operator==(const Struct_B &that) const {
        if (flag_ != that.flag_ || id_ != that.id_ || data_ != that.data_ || !(struct_a_ == that.struct_a_))
            return false;
        return true;
    }
};

bool toJsonValue(const Struct_B &input, rapidjson::Value &output, jsonreflect::Alloc &alloc) {
    if (!jsonreflect::toJsonValue(input.flag_, "flag_", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.id_, "id_", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.data_, "data_", output, alloc)) return false;
    if (!jsonreflect::toJsonValue(input.struct_a_, "struct_a_", output, alloc)) return false;
    return true;
}

bool jsonValueTo(const rapidjson::Value &input, Struct_B &output) {
    if (!jsonreflect::jsonValueTo(input, "flag_", output.flag_)) return false;
    if (!jsonreflect::jsonValueTo(input, "id_", output.id_)) return false;
    if (!jsonreflect::jsonValueTo(input, "data_", output.data_)) return false;
    if (!jsonreflect::jsonValueTo(input, "struct_a_", output.struct_a_)) return false;
    return true;
}
#endif /* __STRUCT_B_H__ */
