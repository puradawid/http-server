#ifndef SERVER_H
#define SERVER_H

#include "dispatcher.h"

/// @brief entry class that controls the server. This is the actual class that user should interact (create and invokes methods) with.
class Server {
private:
    int port;
    Dispatcher mDispatcher;
public:
    /// @brief Creates a server on specific port
    /// @param port port provided, like 8080
    Server(int port);

    /// @brief runs server
    void start();

    /// @brief registers handlers
    void registerHandler(std::shared_ptr<Handler> h);
};

#endif