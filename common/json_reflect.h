// Copyright (c) 2017 macrozhang.
// All rights reserved.
//

#ifndef _JSON_REFLECT_H_
#define _JSON_REFLECT_H_

#define RAPIDJSON_HAS_STDSTRING 1

#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>

#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

namespace jsonreflect {
typedef rapidjson::Document::AllocatorType Alloc;

#define TO_JSON_VALUE(input_type, output_type, cast) \
    bool toJsonValue(const input_type &input, rapidjson::Value &output, Alloc &alloc) { \
        output.Set ## output_type(cast(input)); \
        return true; \
    }

#define JSON_VALUE_TO(input_type, output_type, cast) \
    bool jsonValueTo(const rapidjson::Value &input, output_type &output) { \
        if (input.Is ## input_type()) { \
            output = cast(input.Get ## input_type()); \
            return true; \
        } \
        return false; \
    } 
}

namespace jsonreflect {
    template<typename FROM, typename TO>
    bool transfrom(FROM &from, TO &to, Alloc &alloc) {
		if (from.IsObject()) {
            to.SetObject();
            typename FROM::MemberIterator iter;
            for (iter = from.MemberBegin(); iter != from.MemberEnd(); iter++)
                to.AddMember(rapidjson::StringRef(iter->name.GetString()), iter->value.Move(), alloc); 
        } else if (from.IsArray()) {
			to.SetArray();
            for (rapidjson::SizeType i = 0; i < from.Size(); i++)
                to.PushBack(from[i].Move(), alloc);
        } else if (from.IsBool()) {
            to.SetBool(from.GetBool());
        } else if (from.IsInt()) {
            to.SetInt(from.GetInt());
        } else if (from.IsUint()) {
            to.SetUint(from.GetUint());
        } else if (from.IsInt64()) {
            to.SetInt64(from.GetInt64());
        } else if (from.IsUint64()) {
            to.SetUint64(from.GetUint64());
        } else if (from.IsDouble()) {
            to.SetDouble(from.GetDouble());
        } else if (from.IsString()) {
            to.SetString(rapidjson::StringRef(from.GetString()));
        } else if (from.IsNull()){
            to.SetNull();
        } else {
            return false;
        }

        return true;
    }
}


namespace jsonreflect {
    /* bool,
     * int8_t, uint8_t,
     * int16_t, uint16_t,
     * int32_t, uint32_t,
     * int64_t, uint64_t,
     * float, double,
     * std::string, char* to json value*/

    TO_JSON_VALUE(bool, Bool,)
    TO_JSON_VALUE(int8_t, Int, static_cast<int32_t>)
    TO_JSON_VALUE(uint8_t, Uint, static_cast<uint32_t>)
    TO_JSON_VALUE(int16_t, Int, static_cast<int32_t>)
    TO_JSON_VALUE(uint16_t, Uint, static_cast<uint32_t>)
    TO_JSON_VALUE(int32_t, Int,)
    TO_JSON_VALUE(uint32_t, Uint,)
    TO_JSON_VALUE(int64_t, Int64,)
    TO_JSON_VALUE(uint64_t, Uint64,)
    TO_JSON_VALUE(float, Float,)
    TO_JSON_VALUE(double, Double,)
    TO_JSON_VALUE(std::string, String, rapidjson::StringRef)

    template<typename T>
    bool toJsonValue(const std::vector<T> &input, rapidjson::Value &output, Alloc &alloc) {
        output.SetArray();
        typename std::vector<T>::const_iterator iter;
        for (iter = input.begin(); iter != input.end(); ++iter) {
            rapidjson::Value val;
            if (toJsonValue(*iter, val, alloc))
                output.PushBack(val.Move(), alloc);
            else
                return false;
        }
        return true;
    }

    template<typename T>
    bool toJsonValue(const std::map<std::string, T> &input, rapidjson::Value &output, Alloc &alloc) {
        output.SetObject();
        typename std::map<std::string, T>::const_iterator iter;
        for (iter = input.begin(); iter != input.end(); ++iter) {
            rapidjson::Value val;
            if (toJsonValue(iter->second, val, alloc))
                output.AddMember(rapidjson::StringRef(iter->first.c_str()), val.Move(), alloc);
            else 
                return false;
        }
        return true;
    }

    template<typename T>
    bool toJsonValue(const T& field, const char *name, rapidjson::Value &output, Alloc &alloc) {
        rapidjson::Value val;
        if (!name || !toJsonValue(field, val, alloc))
            return false;
        if (!output.IsObject())
            output.SetObject();

        output.AddMember(rapidjson::StringRef(name), val.Move(), alloc);
        return true;
    }

    template<typename T>
    bool serialize(const T &input, std::string &output) {
        /*
         * 1. T                     -> rapidjson::Document
         * 2. rapidjson::Value      -> rapidjson::Document
         * 3. rapidjson::Document   -> std::string 
         */
        rapidjson::Value val;
        rapidjson::Document doc;
        if (!toJsonValue(input, val, doc.GetAllocator()))
            return false;
        if (!jsonreflect::transfrom<rapidjson::Value, rapidjson::Document>(val, doc, doc.GetAllocator())) 
            return false;

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        output = buffer.GetString();
        return true;
    }
}

namespace jsonreflect {
    JSON_VALUE_TO(Bool, bool,)
    JSON_VALUE_TO(Int, int8_t, static_cast<int8_t>)
    JSON_VALUE_TO(Uint, uint8_t, static_cast<uint8_t>)
    JSON_VALUE_TO(Int, int16_t, static_cast<int16_t>)
    JSON_VALUE_TO(Uint, uint16_t, static_cast<uint16_t>)
    JSON_VALUE_TO(Int, int32_t,)
    JSON_VALUE_TO(Uint, uint32_t,)
    JSON_VALUE_TO(Int64, int64_t,)
    JSON_VALUE_TO(Uint64, uint64_t,)
    JSON_VALUE_TO(Double, float, static_cast<float>)
    JSON_VALUE_TO(Double, double,)
    JSON_VALUE_TO(String, std::string,)

    template<typename T>
    bool jsonValueTo(const rapidjson::Value &input, std::vector<T> &output) {
        if (!input.IsArray())
            return false;
        for (rapidjson::SizeType i = 0; i < input.Size(); ++i) {
            T val; 
            if (jsonValueTo(input[i], val))
                output.push_back(val);
            else    
                return false;
        }

        return true;
    }

    template<typename T>
    bool jsonValueTo(const rapidjson::Value &input, std::map<std::string, T> &output) {
        if (!input.IsObject())
            return false;
        for (rapidjson::Value::ConstMemberIterator iter=input.MemberBegin(); iter!=input.MemberEnd(); ++iter) {
            T val;
            if (jsonValueTo(iter->value, val))
                output[iter->name.GetString()] = val;
            else 
                return false;
        }

        return true;
    }

    template<typename T>
    bool jsonValueTo(const rapidjson::Value &input, const char *name, T &field) {
        if (!name || !input.IsObject())
            return false;
        if (input.HasMember(name)) {
            const rapidjson::Value &val = input[name];
            if (!jsonValueTo(val, field))
                return false;
        }

        return true;
    }

    template<typename T>
    bool deserialize(const std::string &input, T &output) {
        /*
         * 1. std::string           -> rapidjson::Document
         * 2. rapidjson::Document   -> rapidjson::Value 
         * 3. rapidjson::Value      -> T
         */
        rapidjson::Value val;
        rapidjson::Document doc;
        doc.Parse(input.c_str());
        if (!jsonreflect::transfrom<rapidjson::Document, rapidjson::Value>(doc, val, doc.GetAllocator()))
            return false;
        
        if (!jsonValueTo(val, output))
            return false;
        return true; 
    }
}

#endif /* _JSON_REFLECT_H_ */

