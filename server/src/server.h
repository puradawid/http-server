#ifndef SERVER_H
#define SERVER_H

#include "dispatcher.h"

class Server {
private:
    int port;
    Dispatcher mDispatcher;
public:
    Server(int port);
    void start();
    void registerHandler(std::shared_ptr<Handler> h);
};

#endif