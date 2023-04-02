#ifndef CONNECTION_H
#define CONNECTION

#include<string>

class Connection {
private:
    int port;
    int sockfd;
public:
    Connection(int port);
    void open();
    void listen();
    void close();
};

class Listener {
public:
    virtual void receive(std::string msg) = 0;
};

#endif