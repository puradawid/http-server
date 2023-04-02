#include "server.h"
#include "response.h"
#include "connection.h"

#include<iostream>

Server::Server(int port) {
    std::cout << "Server started" << std::endl;
    this->port = port;
};

void Server::start() {
    Connection c(this->port);
    c.open();
    c.listen();
}