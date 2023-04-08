#include "server/src/server.h"
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <csignal>
#include <cstdlib>

Logging::Logger& LOG = Logging::LOG.getLogger("ExampleHandler");

class ExampleHandler : public Handler {
public:

    Response handle(Request& r) {
        LOG.debug("Handling request " + r.path());

        Response resp(Responses::RESP_200);

        resp.headers().addHeader(Header("Host", r.headers().find("Host").value()));

        if (r.path() == "/someTest") {
            resp.headers().addHeader(Header("X-For-Path", r.path()));
        }

        resp.content("<html><h1>Hello my friend, this is your request</h1><p>" + r.content() + "</p></html>", "text/html");

        return resp;
    }
    bool test(Request& r) {
        static const int EXTENSION_SIZE = 5; 
        return r.path().rfind(".html") == r.path().size() - EXTENSION_SIZE;
    }
};

class ExampleJSONHandler : public Handler {
public:

    Response handle(Request& r) {
        LOG.debug("Handling request " + r.path());

        Response resp(Responses::RESP_200);

        resp.headers().addHeader(Header("Host", r.headers().find("Host").value()));

        resp.content("{ path: \"" + r.path() + "\" }", "application/json");

        return resp;
    }
    bool test(Request& r) {
        static const int EXTENSION_SIZE = 5; 
        return r.path().rfind(".json") == r.path().size() - EXTENSION_SIZE;
    }
};

void run(Server* s) {
    s->start();
}

int main(int i, char** argv) {
    int port = i == 2 ? atoi(argv[1]) : 8080;

    Server s(port);
    s.registerHandler(std::make_shared<ExampleHandler>());
    s.registerHandler(std::make_shared<ExampleJSONHandler>());

    std::thread t(run, &s);

    std::cout << "Thread started" << std::endl;

    t.join();
}