#include "dispatcher.h"
#include "parser.h"

void Dispatcher::add(std::shared_ptr<Handler> handler)
{
    this->mHandlers.push_back(handler);
}

void handle(Request& r, Handler& h, Connection& c) {
    Response response = h.handle(r);
    c.write(response);
    c.close();
}

Response ERROR = Response(Responses::RESP_500);

void Dispatcher::onOpenedConnection(Connection &conn)
{
    HttpParser p;
    
    Continue c = true;    
    do {
        c = p.digest(conn.read());
    } while (c.continueProcessing);

    if (c.errorMessage != "") {
        conn.write(ERROR);
        conn.close();
    }

    Request r = p.build();

    if (this->mHandlers.size() == 0) {
        conn.close();
        return;
    }

    for (std::shared_ptr<Handler> handler : this->mHandlers) {
        if (handler->test(r)) {
            handle(r, *handler, conn);
            return; // only one handler can be invoked
        }
    }
    handle(r, *(mHandlers.front()), conn); // execute default handler (first one)
}
