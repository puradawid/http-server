#ifndef CONNECTION_H
#define CONNECTION

#include<string>
#include "parser.h"
#include "serializable.h"

class Connection {
    virtual MessageChunk read() = 0;
    virtual void write(Serializable s) = 0;
    virtual void close() = 0;
};

class TcpConnection : public Connection {
private:
    int port;
    int sockfd;
    int newsockfd;
public:
    TcpConnection(int port);
    void open();
    void listen();
    MessageChunk read();
    void write(Serializable s);
    void close();
};

class Listener {
public:
    virtual void receive(std::string msg) = 0;
};

class IncomingConnectionObserver {
    virtual void onOpenedConnection(Connection* conn) = 0;
};

class PortListener {
    virtual void registerObserver(IncomingConnectionObserver* observer) = 0;
    virtual void unregisterObserver(IncomingConnectionObserver* observer) = 0;
    virtual void listen() = 0;
    virtual void close() = 0;
};

#endif