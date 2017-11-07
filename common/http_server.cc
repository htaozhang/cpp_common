// Copyright(c) 2017 macrozhang
// All rights reserved.
//
//

#include <assert.h>

#include "http_server.h"

///
/// HttpRequest
///
const char* HttpRequest::VersionString() const {
    const char* result = "UNKNOWN";
    switch (version_) {
        case kHttp10:
            result = "HTTP/1.0";
            break;
        case kHttp11:
            result = "HTTO/1.1";
            break;
        default:
            break;
    }

    return result;
}

bool HttpRequest::SetMethod(const char* from, const char* to) {
    assert(method_ == kInvalid);

    std::string method(from, to);

    if (method == "GET")
        method_ = kGet;
    else if (method == "POST")
        method_ = kPost;
    else if (method == "HEAD")
        method_ = kHead;
    else if (method == "PUT")
        method_ = kPut;
    else if (method == "DELETE")
        method_ = kDelete;
    else
        method_ = kInvalid;

    return method_ != kInvalid;
}

const char* HttpRequest::MethodString() const {
    const char* result = "UNKNOWN";
    switch (method_) {
        case kGet:
            result = "GET";
            break;
        case kPost:
            result = "POST";
            break;
        case kHead:
            result = "HEAD";
            break;
        case kPut:
            result = "PUT";
            break;
        case kDelete:
            result = "DELETE";
            break;
        default:
            break;
    }

    return result;
}

void HttpRequest::SetHeader(const char* from, const char* colon, const char* to) {
    std::string field(from, colon);
    for (++colon; colon < to && isspace(*colon); colon++)
        ;
    std::string value(colon, to);
    for (; !value.empty() && isspace(value[value.size()-1]); value.resize(value.size() - 1))
        ;

    headers_[field] = value;
}

std::string HttpRequest::GetHeader(const std::string& field) const {
    std::string result;
    std::map<std::string, std::string>::const_iterator iter = headers_.find(field);
    if (iter != headers_.end())
        result = iter->second;
    return result;
}

void HttpRequest::Swap(HttpRequest& that) {
    std::swap(version_, that.version_);
    std::swap(method_, that.method_);
    path_.swap(that.path_);
    query_.swap(that.query_);
    headers_.swap(that.headers_);
}

///
/// HttpContext
///
bool HttpContext::ParseRequest(const char* from, const char* to) {
    bool over = false;
    const char* start = from;
    const char* crlf = NULL;

    while (over && start < to) {
        switch (cursor_) {
        case kRequestLine:
            crlf = std::search(start, to, "\r\n", "\r\n" + 2);
            if (crlf && ParseRequestLine(start, crlf)) {
                cursor_ = kHeader;
                start = crlf + 2;
            } else {
                // over = true;
                return crlf == NULL;
            }
            break;
        case kHeader:
            if (crlf = std::search(start, to, "\r\n", "\r\n" + 2)) {
                const char* colon = std::find(start, crlf, ':');
                if (colon != crlf) {
                    request_.SetHeader(start, colon, crlf);
                } else if (request_.GetMethod() == HttpRequest::kPost) {
                    //size = atoi(request_.GetHeader("Conten-Length").c_str());
                    cursor_ = kBody;
                } else {
                    over = true;
                    cursor_ = kAll;
                }
                start = crlf + 2;
            } else {
                over = true;
            }
            break;
        case kBody:
            //request_.SetQuery(buff, buff + len);
            break;
        default:
            over = true;
            break;
        }
    }

    //HttpRequest::Print(&request_);

    return true;
}

bool HttpContext::ParseRequestLine(const char* from, const char* to) {
    // [method][ ][url][ ][version]
    bool succeed = false;
    const char* start = from;
    const char* space = std::find(start, to, ' ');

    if (space != to && request_.SetMethod(start, space)) {
        start = space + 1;
        space = std::find(start, to, ' ');
        if (space != to) {
            const char* path = std::find(start, space, '?');
            request_.SetPath(start, path);
            if (path != space)
                request_.SetQuery(path, space);
        }

        start = space + 1;
        succeed = (to - start == 8) && std::equal(start, to - 1, "HTTP/1.");
        if (succeed) {
            if (*(to - 1) == '0')
                request_.SetVersion(HttpRequest::kHttp10);
            else if (*(to - 1) == '1')
                request_.SetVersion(HttpRequest::kHttp11);
            else
                succeed = false;
        }
    }

    return succeed;
}

///
/// HttpResponse
///
void HttpResponse::AppendToBuffer(std::string* output) const {
    char buff[32];
    snprintf(buff, sizeof buff, "HTTP/1.1 %d ", statuscode_);
    output->append(buff);
    output->append(statusmessage_);
    output->append("\r\n");

    if (close_) {
        output->append("Connection: close\r\n");
    } else {
        snprintf(buff, sizeof buff, "Content-Length: %zd\r\n", body_.size());
        output->append(buff);
        output->append("Connection: Keep-Alive\r\n");
    }

    for (std::map<std::string, std::string>::const_iterator iter = headers_.begin(); 
        iter != headers_.end(); iter++) {
        output->append(iter->first);
        output->append(": ");
        output->append(iter->second);
        output->append("\r\n");
    }

    output->append("\r\n");
    output->append(body_);
}

void HttpResponse::BadRequest(HttpResponse* response) {
    response->SetStatusCode(HttpResponse::k400BadRequest);
    response->SetStatusMessage("Bad request");
    response->SetContentType("text/html");
    response->SetBody("bad request.\n");
}

void HttpResponse::NotFound(HttpResponse* response) {
    response->SetStatusCode(HttpResponse::k404NotFound);
    response->SetStatusMessage("Not found");
    response->SetContentType("text/html");
    response->SetBody("not found.\n");
}

void HttpResponse::Unimplemented(HttpResponse* response) {
    response->SetStatusCode(HttpResponse::k501Unimplemented);
    response->SetStatusMessage("Method Not Implemented");
    response->SetContentType("text/html");
    response->SetBody("method not implemented.\n");
}




///
/// HttpServer
///
HttpServer::HttpServer(int16_t port) {
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(port);
}

HttpServer::HttpServer(const std::string& ip, int16_t port) {
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
    addr_.sin_port = htons(port);
}

int HttpServer::Startup() {
    initsocket();

    if ((listenfd_ = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return -1;
    }

    int opt = 0;
    if (setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
        return -2;
    }

    if (::bind(listenfd_, (struct sockaddr*)&addr_, sizeof(addr_)) < 0) {
        std::cerr << "bind error: " << strerror(errno) << std::endl;
        return -3;
    }

    if (listen(listenfd_, 5) < 0) {
        std::cerr << "listen error: " << strerror(errno) << std::endl;
        return -4;
    }

    return 0;
}

void HttpServer::Run() {
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int connectfd = -1;

    while (1) {
        if ((connectfd = accept(listenfd_, (struct sockaddr*)&clientaddr, &len)) == -1) {
            std::cerr << "accept error: " << strerror(errno) << std::endl;
            continue;
        }

        Process(connectfd);
    }
}

bool HttpServer::Process(int fd) {
    char buff[MAX_SIZE] = {0};
    bool over = false;
    ParseRequestFlag flag = kRequestLine;
    HttpRequest* request = new HttpRequest();
    HttpResponse* response = new HttpResponse(); 
    
    for (size_t size = sizeof(buff); !over; ) {
        int len = Recv(fd, buff, size);
        if (len <= 0) {
            over = true;
            continue;
        }

        if (flag == kRequestLine) {
            if (ParseRequest(request, buff, buff + len)) {
                flag = kHeader;
            } else {
                HttpResponse::BadRequest(response);
                over = true;
            }
        } else if (flag == kHeader) {
            const char* colon = std::find(buff, buff + len, ':');
            if (colon != buff + len) {
                request->SetHeader(buff, colon, buff + len);
            } else if (request->GetMethod() == HttpRequest::kPost) {
                size = atoi(request->GetHeader("Conten-Length").c_str());
                flag = kBody;
            } else {
                over = true;
                flag = kAll;
            }
        } else if (flag == kBody) {
            request->SetQuery(buff, buff + len);
        } else {
            over = true;
        }
    }

    if (callbacks_.find(request->GetPath()) != callbacks_.end()) {
        callbacks_[request->GetPath()](request, response);
    } else {
        HttpResponse::NotFound(response);
    }

    HttpRequest::Print(request);
    HttpResponse::Print(response);

    Send(fd, response);
    
    delete request;
    delete response;
    return true;
}

bool HttpServer::ParseRequest(HttpRequest* request, const char* from, const char* to) {
    // [method][ ][url][ ][version]

    bool succeed = false;
    const char* start = from;
    const char* space = std::find(start, to, ' ');

    if (space != to && request->SetMethod(start, space)) {
        start = space + 1;
        space = std::find(start, to, ' ');
        if (space != to) {
            const char* path = std::find(start, space, '?');
            request->SetPath(start, path);
            if (path != space)
                request->SetQuery(path, space);
        }

        start = space + 1;
        succeed = (to - start == 8) && std::equal(start, to - 1, "HTTP/1.");
        if (succeed) {
            if (*(to - 1) == '0')
                request->SetVersion(HttpRequest::kHttp10);
            else if (*(to - 1) == '1')
                request->SetVersion(HttpRequest::kHttp11);
            else
                succeed = false;
        }

    }

    return succeed;
}

int HttpServer::Recv(int fd, char* buff, size_t size) {
    int i = 0;
    char c = '\0';
    ssize_t n = 0;

    while ((i + 1 < (int)size) && (c != '\n')) {
        if ((n = recv(fd, &c, 1, 0)) <= 0) {
            c = '\n';
        } else {
            if (c == '\r') {
                /* 
                 * This MSG_PEEK causes the receive operation to 
                 * return data from the beginning of the receive queue
                 * without removing that data from the queue.  
                 * Thus, a subsequent receive call will return the same data.
                 */
                if ((n = recv(fd, &c, 1, MSG_PEEK)) > 0 && c == '\n')
                    n = recv(fd, &c, 1, 0);
                else
                    c = '\n';
            }
            buff[i++] = c;
        } 
    }

    buff[i] = '\0';
    return i;
}


ssize_t HttpServer::Send(int fd, HttpResponse* response) {
    std::string data;
    response->AppendToBuffer(&data);
    return send(fd, data.c_str(), data.size(), 0); 
}

