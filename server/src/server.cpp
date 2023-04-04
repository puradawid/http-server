#include "server.h"
#include "response.h"
#include "connection.h"

#include<iostream>

Server::Server(int port) {
    std::cout << "Server started" << std::endl;
    this->port = port;
};

void Server::start() {
    TcpConnection c(this->port);
    c.open();
    c.listen();
    
    HttpParser p;
    Continue more(true);
    while(more.continueProcessing) {
        MessageChunk chunk = c.read();
        more = p.digest(chunk);
    }
    Request r = p.build();
    std::cout << r.header("accept") << std::endl;
    c.close();
}