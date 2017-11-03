
#include <cstdio>

#include "http_server.h"

#include "gtest/gtest.h"

int OnQuery(const HttpRequest* request, HttpResponse* response) {
    response->SetStatusCode(HttpResponse::k200Ok);
    response->SetStatusMessage("OK");
    response->SetContentType("text/html");
    response->SetBody("hello world.\n");
    return 0;
}


int main() {
    HttpServer server(23333);
    server.SetCallback("/query", OnQuery);

    if (server.Startup()) {
        printf("server startup failed.\n");
        return -1;
    }

    server.Run();

    return 0;
}



