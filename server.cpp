#include "server/src/server.h"
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <csignal>

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

        resp.content("{}", "application/json");

        return resp;
    }
    bool test(Request& r) {
        return true;
    }
};

void run(Server* s) {
    s->start();
}

int main(int i, char** argv) {
    int port = i == 2 ? atoi(argv[1]) : 8080;

    Server s(port);
    s.registerHandler(std::make_shared<ExampleHandler>());

    std::thread t(run, &s);

    std::cout << "Thread started" << std::endl;

    t.join();
}