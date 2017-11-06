// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <functional>

#include <cstring>

#include "generic_socket.h"

class HttpRequest {
public:
    enum Version { kUnknown, kHttp10, kHttp11 };
    enum Method { kInvalid, kGet, kPost, kHead, kPut, kDelete };

    HttpRequest() : version_(kUnknown), method_(kInvalid) {}
    
    void SetVersion(Version v) { version_ = v; }
    Version GetVersion() const { return version_; }
    const char* VersionString() const;

    bool SetMethod(const char* from, const char* to);
    Method GetMethod() const { return method_; }
    const char* MethodString() const;

    void SetPath(const char* from, const char* to) { path_.assign(from, to); }
    const std::string& GetPath() const { return path_; }

    void SetQuery(const char* from, const char* to) { query_.assign(from, to); }
    const std::string& GetQuery() const { return query_; }

    void SetHeader(const char* from, const char* colon, const char* to);
    std::string GetHeader(const std::string& field) const;

    const std::map<std::string, std::string>& Headers() const { return headers_; }

    void Swap(HttpRequest& that);

    static void Print(HttpRequest* request) {
        printf("%s %s%s %s\r\n", request->MethodString(), 
            request->path_.c_str(), request->query_.c_str(), request->VersionString());
        for (std::map<std::string, std::string>::iterator 
            iter = request->headers_.begin(); iter != request->headers_.end(); iter++) {
            printf("%s: %s\r\n", iter->first.c_str(), iter->second.c_str());
        }
        printf("\r\n");
    }

private:
    Version version_;
    Method method_;
    std::string path_;
    std::string query_;
    std::map<std::string, std::string> headers_;
};

class HttpContext {
public:
    enum ParseCursor { kRequestLine = 1, kHeader = 2, kBody = 4, kAll = 8 };

    HttpContext() : cursor_(kRequestLine) {}
    bool ParseRequest(const char* from, const char* to) { return true; }
    void Reset() {
        cursor_ = kRequestLine;
        HttpRequest dummy;
        request_.Swap(dummy);
    }
    const HttpRequest& Request() const { return request_; }
    HttpRequest& Request() { return request_; }

private:
    bool ParseRequestLine(const char* from, const char* to) { return true; }

    HttpRequest request_;
    ParseCursor cursor_;
};

class HttpResponse {
public:
    enum StatusCode {
        kUnknown, 
        k200Ok = 200, 
        k301MovedPermanently = 301, 
        k400BadRequest = 400,
        k404NotFound = 404,
        k501Unimplemented = 501,
    };

    explicit HttpResponse(bool close = false) : statuscode_(kUnknown), close_(close) {}
    
    static void BadRequest(HttpResponse* response);
    static void NotFound(HttpResponse* response);
    static void Unimplemented(HttpResponse* response);

    void SetStatusCode(StatusCode statuscode) { statuscode_ = statuscode; }
    void SetStatusMessage(const std::string& statusmessage) { statusmessage_ = statusmessage; }
    void SetClose(bool close) { close_ = close; }
    void SetBody(const std::string& body) { body_ = body; }
    void SetHeader(const std::string& key, const std::string& value) { headers_[key] = value; }
    void SetContentType(const std::string& contenttype) { SetHeader("Content-Type", contenttype); }
    void AppendToBuffer(std::string* output) const;
    
    static void Print(HttpResponse* response) {
       std::string data;
       response->AppendToBuffer(&data);
       printf("%s", data.c_str());
    }

private:
    StatusCode statuscode_;
    std::string statusmessage_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    bool close_;
};

class HttpServer {
public:
    enum ParseRequestFlag { kRequestLine = 1, kHeader = 2, kBody = 4, kAll = 8 };
    typedef std::function<int (const HttpRequest* request, HttpResponse* response)> Callback;
    
    explicit HttpServer(int16_t port);
    HttpServer(const std::string& ip, int16_t port);
    int Startup();
    void Run();
    void SetCallback(const std::string& key, Callback callback) { callbacks_[key] = callback; }

private:
    bool Process(int fd);
    bool ParseRequest(HttpRequest* request, const char* from, const char* to);
    int Recv(int fd, char* buff, size_t size);
    ssize_t Send(int fd, HttpResponse* response);

private:
    struct sockaddr_in addr_;
    int listenfd_;
    std::map<std::string, Callback> callbacks_;
    static const int MAX_SIZE = 1024;
};

#endif /* __HTTP_SERVER_H__ */
