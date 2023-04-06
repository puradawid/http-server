#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "connection.h"
#include "response.h"

/// @brief interface that provides handling for requests/response
class Handler {
public:
    virtual Response handle(Request& request) = 0;
    virtual bool test(Request& request) = 0;
};

class Dispatcher : public IncomingConnectionObserver {
private:
    std::list<std::shared_ptr<Handler>> mHandlers;
public:
    void add(std::shared_ptr<Handler> handler);
    void onOpenedConnection(Connection& conn);
};

#endif