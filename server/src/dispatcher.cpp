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

Logging::Logger& Dispatcher::LOG = Logging::LOG.getLogger("dispatcher");

void Dispatcher::onOpenedConnection(Connection &conn)
{
    HttpParser p;
        
    Continue c = true;
    try {
        // this section is hard logged in order to store the information about logging and also demo the use case of logger
        Dispatcher::LOG.debug("Starting main loop for receiving the request");
        do {
            Dispatcher::LOG.debug("Reading a chunk of data");
            MessageChunk chunk = conn.read();
            Dispatcher::LOG.debug("Chunk of data: " + chunk.message());
            if (chunk.message() == "") {
                Dispatcher::LOG.warn("Client has sent empty message?");
                c = Continue("empty message sent");
            } else {
                Dispatcher::LOG.debug("Sending the request for parser");
                c = p.digest(chunk);
            }
        } while (c.continueProcessing);
        
        Dispatcher::LOG.debug("The incoming message processed with '" + c.errorMessage + "' error message");

        if (c.errorMessage != "") {
            conn.write(ERROR);
            conn.close();
        }

        Request r = p.build();

        if (this->mHandlers.size() == 0) {
            Dispatcher::LOG.error("There is no handlers to handle this request! Closing the connection.");
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
    } catch (ConnectionError& e) {
        Dispatcher::LOG.warn("A connection error has occured. Closing connection");
        conn.close();
    }
}
