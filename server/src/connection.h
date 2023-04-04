#ifndef CONNECTION_H
#define CONNECTION

#include <string>
#include <thread>
#include "parser.h"
#include "serializable.h"

class Connection {
public:
    virtual MessageChunk read() = 0;
    virtual void write(Serializable s) = 0;
    virtual void close() = 0;
};

class TcpConnection : public Connection {
private:
    int newsockfd;
public:
    TcpConnection(int newsockfd);
    MessageChunk read();
    void write(Serializable s);
    void close();
};

class Listener {
public:
    virtual void receive(std::string msg) = 0;
};

class IncomingConnectionObserver {
public:
    virtual void onOpenedConnection(Connection* conn) = 0;
};

class PortListener {
    virtual void registerObserver(IncomingConnectionObserver* observer) = 0;
    virtual void unregisterObserver(IncomingConnectionObserver* observer) = 0;
    virtual void listen() = 0;
    virtual void close() = 0;
};

class TcpPortListener : public PortListener {
private:
    int mPort;
    int mSockfd;
    std::thread* t;
    IncomingConnectionObserver* mObserver;
public:
    TcpPortListener(int port);
    void registerObserver(IncomingConnectionObserver* observer);
    void unregisterObserver(IncomingConnectionObserver* observer);
    void listen();
    void close();
};

#endif