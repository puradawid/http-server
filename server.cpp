#include "server/src/server.h"
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <csignal>

void run(Server* s) {
    s->start();
}

int main(int i, char** argv) {
    int port = i > 0 ? atoi(argv[1]) : 8080;

    Server s(port);

    std::thread t(run, &s);

    std::cout << "Thread started" << std::endl;

    t.join();
}