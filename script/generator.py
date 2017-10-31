#coding: utf-8

import os
import sys 
import time
import json
import string
import datetime

TYPE_IDX = 0
NAME_IDX = 1
VALUE_IDX = 2

class Generator:
    def __init__(self, infile, outpath, bin):
        self.__infile = infile
        self.__outpath = outpath
        self.__bin = bin
        self.__scalar = lambda key: key[1: len(key) - 1]
        self.__object = lambda key: key.startswith('{') and key.endswith('}')
        self.__array = lambda key: key.startswith('[') and key.endswith(']')
        self.__list = lambda arr: string.join(arr, '\n')

        self.__tojsondoc = {
            "dec": lambda name: 
                "bool toJsonValue(const %s &input, rapidjson::Value &output, jsonreflect::Alloc &alloc)" % name,
            "imp": lambda members: self.__list([
                '    if (!jsonreflect::toJsonValue(input.%s, "%s", output, alloc)) return false;' %(
                    member[NAME_IDX], member[NAME_IDX]) for member in members])
        }
        self.__jsondocto = {
            "dec": lambda name: "bool jsonValueTo(const rapidjson::Value &input, %s &output)" % name,
            "imp": lambda members: self.__list([
                '    if (!jsonreflect::jsonValueTo(input, "%s", output.%s)) return false;' %(
                    member[NAME_IDX], member[NAME_IDX]) for member in members])
        }

    def __load(self):
        return json.load(open(self.__infile, 'r'))

    def __type(self, key):
        if (self.__object(key)):
            return "std::map<std::string, %s>" % (self.__type(self.__scalar(key)))
        elif (self.__array(key)):
            return "std::vector<%s>" % (self.__type(self.__scalar(key)))
        elif (key == "string"):
            return "std::string"
        return key
    
    def __member(self, S):
        for struct in S["structs"]:
            for member in struct["members"]:
                member[TYPE_IDX] = self.__type(member[TYPE_IDX])
                pass
            pass
    
    def __define(self, struct):
        return self.__list([
            "struct %s {" % (struct["name"]),
            self.__list(["    %s %s;" % (member[TYPE_IDX], member[NAME_IDX]) for member in struct["members"]]),
            "};"
            ])

    def __method(self, name, members):
        return self.__list([
            self.__list([
                self.__tojsondoc["dec"](name) + ' {', self.__tojsondoc["imp"](members), "    return true;", '}']),
            '',
            self.__list([
                self.__jsondocto["dec"](name) + ' {', self.__jsondocto["imp"](members), "    return true;", '}']),
            '\n'
            ])

    def __writer(self, name, define, method):
        macro = '__%s_H__' % (name.upper())
        header = '// Copyright (c) %s. All rights reserved' % (datetime.datetime.now().strftime('%Y'))
        
        data = self.__list([
            header,
            '#ifndef %s' % macro,
            '#define %s' % macro,
            '',
            define,
            '',
            method,
            '#endif /* %s */' % macro,
            ''
            ])
        print data
        if (self.__bin == True):
            open(
                "%s/%s.h" % (self.__outpath[0:-1] if self.__outpath.endswith('/') else self.__outpath, name),
                'w').writelines(data)

    def start(self):
        S = self.__load()
        self.__member(S)
        
        for struct in S["structs"]:
            self.__writer(
                struct["name"],
                self.__define(struct),
                self.__method(struct["name"], struct["members"])
                )
            pass
    
def main():
    if len(sys.argv) < 4:
        print 'Usage: python generator.py [inputfile] [outpath] [debug/bin]'
        exit(-1)

    generator = Generator(sys.argv[1], sys.argv[2], True if (sys.argv[3] == "bin") else False)
    generator.start();
    return

if __name__ == "__main__":
    main()


