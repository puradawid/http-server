#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "connection.h"
#include "response.h"

/// @brief interface that provides handling for requests/response
class Handler {
public:
    /// @brief Handles the request and creates a response object. This may be run by multiple threads, so has to be thread-safe method
    /// @param request a request to read from (do not modify the request!)
    /// @return a response (new instance)
    virtual Response handle(Request& request) = 0;
    /// @brief returns true if the handler may handle the request. REMEMBER: if there is only one handler, all requests will be handled
    ///        anyway
    /// @param request a request object (do not modify it!) 
    /// @return true if request can be handled, false otherwise
    virtual bool test(Request& request) = 0;
};

/// @brief a class that dispatches the incoming requests to apropriate connections, notified by PortListener
class Dispatcher : public IncomingConnectionObserver {
private:
    static Logging::Logger& LOG;
    std::list<std::shared_ptr<Handler>> mHandlers;
public:
    /// @brief adds a new handler for handling requests
    /// @param handler shared pointer to handler object. It can be withhold by many threads! 
    void add(std::shared_ptr<Handler> handler);

    /// @brief notification of opened connection (this implements IncomingConnectionObserver)
    /// @param conn opened connection
    void onOpenedConnection(Connection& conn);
};

#endif