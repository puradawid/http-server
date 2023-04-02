#ifndef SERVER_H
#define SERVER_H

class Server {
private:
    int port;
public:
    Server(int port);
    void start();
};

#endif