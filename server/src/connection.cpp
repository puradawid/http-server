#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>

TcpConnection::TcpConnection(int port) {
    this->port = port;
};

void error(std::string msg) {
    std::cout << msg << std::endl;
};

void TcpConnection::listen() {
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    // 5 connections tops for this one
    ::listen(this->sockfd,5);

    clilen = sizeof(cli_addr);
    this->newsockfd = accept(this->sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
    if (this->newsockfd < 0) 
        error("ERROR on accept");
}
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

void TcpConnection::open() {
    int portno;
    struct sockaddr_in serv_addr;
    
    // get file descriptor for socket
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if OS decided to not give a process socket
    if (this->sockfd < 0) 
        error("ERROR opening socket");
    
    // clearing serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = this->port;
    
    // create configuration for port infomration
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // register socker to specific port number
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    this->sockfd = sockfd;
};

void TcpConnection::close() {
    std::cout << "Closing connection now" << std::endl;
    ::shutdown(newsockfd, SHUT_RD);
    ::close(newsockfd);
    std::cout << "Closing server connection now" << std::endl;
    ::shutdown(newsockfd, SHUT_RDWR);
    ::close(this->sockfd);
};