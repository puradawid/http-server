#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <thread>
#include "parser.h"
#include "serializable.h"
#include "log.h"

/// @brief Connection error - in case of the connection has been interrupted in a 
/// unknown way and has to be closed
struct ConnectionError : public std::exception {};

/// @brief Class representing connection, can read data from it and write to it, and eventually close it.
class Connection {
public:
    /// @brief Reading a message chunk (that contains at this level essentially a string). The idea of chunks is the assumtion
    ///        this is not a final form of the message yet (it may require some continuation)
    /// @return MessageChunk's object representing the data chunk
    virtual MessageChunk read() = 0;

    /// @brief Writes back a serializable object to the connection. This requires to be the entire object (message).
    /// @param s any serializable object (see Serializable for more information)
    virtual void write(Serializable& s) = 0;

    /// @brief Closes connection, WARNING - after this method execution the connection is no longer usable!
    virtual void close() = 0;
};

/// @brief a Connection that uses TCP as communication channel
class TcpConnection : public Connection {
private:
    static Logging::Logger& LOG;
    int newsockfd; /// SOCK file descriptor of opened connection
public:
    /// @brief Creates a connection with already opened SOCK file descriptor
    /// @param newsockfd opened connection
    TcpConnection(int newsockfd);

    /// @brief Reads the chunk from connection, see Connection::read
    /// @return chunk obtained from connection
    MessageChunk read();

    /// @brief Writes to the connection, see Connection::write
    /// @param s serializable object
    void write(Serializable& s);

    /// @brief Closes connection, see Connection::close
    void close();
};

/// @brief An interface that gets notified when connection gets opened.
class IncomingConnectionObserver {
public:

    /// @brief method executed when connection gets opened
    /// @param conn connection, already opened and ready to interact with
    virtual void onOpenedConnection(Connection& conn) = 0;
};

/// @brief an interface of object that interacts with IncomingConnectionObserver - registers all observers and notifies them
///        when there is an incoming connection from some source
class PortListener {
    /// @brief registers the observer
    /// @param observer a pointer to observer
    virtual void registerObserver(IncomingConnectionObserver* observer) = 0;

    /// @brief undegisters previously registered observer. If observer not registered so far - no action taken
    /// @param observer a pointer to observer
    virtual void unregisterObserver(IncomingConnectionObserver* observer) = 0;

    /// @brief runs the process of listening
    virtual void listen() = 0;

    /// @brief closes all notifications and listetning as well
    virtual void close() = 0;
};

/// @brief a TCP implementation of PortListener
class TcpPortListener : public PortListener {
private:
    static Logging::Logger& LOG;
    int mPort; /// port on which it listens for connections
    int mSockfd; /// a SOCK fd descriptor of listening
    std::thread* t; /// pointer to thread
    IncomingConnectionObserver* mObserver; /// observer
public:
    /// @brief creates the listener listening on specific port
    /// @param port a number of port, for instance 8080
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

/// @brief testing connection that registers the written response to it
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