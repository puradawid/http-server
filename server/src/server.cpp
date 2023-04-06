#include "server.h"
#include "response.h"
#include "connection.h"

#include<iostream>

Server::Server(int port) {
    std::cout << "Server started" << std::endl;
    this->port = port;
};

void Server::start() {
    TcpPortListener tcpListener(this->port);

    tcpListener.registerObserver(&(this->mDispatcher));

    tcpListener.listen();
}

void Server::registerHandler(std::shared_ptr<Handler> h)
{
    this->mDispatcher.add(h);
}
