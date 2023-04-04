#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>

TcpConnection::TcpConnection(int newsockfd) {
    this->newsockfd = newsockfd;
};

void error(std::string msg) {
    std::cout << msg << std::endl;
};

MessageChunk TcpConnection::read()
{
    char buffer[512];
    bzero(buffer,512);

    int n; 
    n = ::read(this->newsockfd,buffer,512);
    if (n < 0) error("ERROR reading from socket");
    return MessageChunk(buffer, n);
}
void TcpConnection::write(Serializable s){};

void TcpConnection::close() {
    std::cout << "Closing connection now" << std::endl;
    ::shutdown(this->newsockfd, SHUT_RD);
    ::close(this->newsockfd);;
};

TcpPortListener::TcpPortListener(int port)
{
    this->mPort = port;
}

void TcpPortListener::registerObserver(IncomingConnectionObserver *observer)
{
    this->mObserver = observer;
}

void TcpPortListener::unregisterObserver(IncomingConnectionObserver *observer)
{
    this->mObserver = 0;
}

void notify(Connection* conn, IncomingConnectionObserver* observer) {
    observer->onOpenedConnection(conn);
}

void listenThread(int sockfd, IncomingConnectionObserver* observer) {
    
    // 5 connections tops for this one
    ::listen(sockfd,5);
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    while (true) {
        int newsockfd;
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        new std::thread(notify, new TcpConnection(newsockfd), observer);
    }
}

void TcpPortListener::listen()
{
    int portno;
    struct sockaddr_in serv_addr;
    
    // get file descriptor for socket
    this->mSockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if OS decided to not give a process socket
    if (this->mSockfd < 0) 
        error("ERROR opening socket");
    
    // clearing serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = this->mPort;
    
    // create configuration for port infomration
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // register socker to specific port number
    if (bind(this->mSockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    this->t = new std::thread(listenThread, this->mSockfd, this->mObserver);
    this->t->join();
}

void TcpPortListener::close()
{
    std::cout << "Closing connection now" << std::endl;
    ::shutdown(this->mSockfd, SHUT_RD);
    ::close(this->mSockfd);
}
