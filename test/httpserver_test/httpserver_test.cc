
#include <cstdio>

#include "http_server.h"

#include "gtest/gtest.h"


TEST(HttpContext, ParseRequest) {
    const char* from = 
        "GET /index.html HTTP/1.1\r\n"
        "Host: www.macrozhang.com\r\n"
        "\r\n";

    HttpContext context;
    EXPECT_TRUE(context.ParseRequest(from, from + strlen(from) + 1));

    const HttpRequest& request = context.Request();
    EXPECT_EQ(request.GetMethod(), HttpRequest::kGet);
    EXPECT_STREQ(request.GetPath().c_str(), "/index.html");
    EXPECT_EQ(request.GetVersion(), HttpRequest::kHttp11);
    EXPECT_EQ(request.GetHeader("Host"), "www.macrozhang.com");
    EXPECT_EQ(request.GetHeader("User-Agent"), "");
}

int OnQuery(const HttpRequest* request, HttpResponse* response) {
    response->SetStatusCode(HttpResponse::k200Ok);
    response->SetStatusMessage("OK");
    response->SetContentType("text/html");
    response->SetBody("hello world.\n");
    return 0;
}


/*
TEST(HttpServer, Startup) {
HttpServer server(23333);
server.SetCallback("/query", OnQuery);
EXPECT_TRUE(server.Startup() == 0);
server.Run();
}
*/





