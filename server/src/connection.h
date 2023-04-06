#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <thread>
#include "parser.h"
#include "serializable.h"
#include "logger.h"

class Connection {
public:
    virtual MessageChunk read() = 0;
    virtual void write(Serializable& s) = 0;
    virtual void close() = 0;
};

class TcpConnection : public Connection {
private:
    static Logging::Logger& LOG;
    int newsockfd;
public:
    TcpConnection(int newsockfd);
    MessageChunk read();
    void write(Serializable& s);
    void close();
};

class Listener {
public:
    virtual void receive(std::string msg) = 0;
};

class IncomingConnectionObserver {
public:
    virtual void onOpenedConnection(Connection& conn) = 0;
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

/// @brief a fake listener used for unit testing. Use send() when need to notify the observer
class TestPortListener : public PortListener {
private:
    IncomingConnectionObserver* mObserver;
    std::string mRequest;
public:
    /// @brief A fake port listener - sends a connection that can be validated for unit testing
    /// @param mObserver observer that is getting notified
    /// @param request a message put in MessageChunk that is sent through the created TestConnection
    TestPortListener(IncomingConnectionObserver* mObserver, std::string request);
    void registerObserver(IncomingConnectionObserver* observer);
    void unregisterObserver(IncomingConnectionObserver* observer);
    void listen();
    void close();

    /// @brief this method triggers notification of observer with incoming message
    std::string send();
};

class TestConnection : public Connection {
private:
    std::string mResponse;
    std::string mRequest;
    bool mClosed;
public:
    TestConnection(std::string request);
    MessageChunk read();
    void write(Serializable& s);
    void close();
    std::string response();
    bool closed();
};

#endif