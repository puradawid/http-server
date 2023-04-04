#include "server.h"
#include "response.h"
#include "connection.h"

#include<iostream>

Server::Server(int port) {
    std::cout << "Server started" << std::endl;
    this->port = port;
};

class ConcreteObserver : public IncomingConnectionObserver {

    void onOpenedConnection(Connection* conn) {
        HttpParser p;
        Continue more(true);
        while(more.continueProcessing) {
            MessageChunk chunk = conn->read();
            more = p.digest(chunk);
        }
        Request r = p.build();
        std::cout << r.header("accept") << std::endl;
        conn->close();
    }

};

void Server::start() {
    TcpPortListener tcpListener(this->port);
    ConcreteObserver co;

    tcpListener.registerObserver(&co);

    tcpListener.listen();
}