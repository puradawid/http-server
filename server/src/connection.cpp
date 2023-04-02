#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>

Connection::Connection(int port) {
    this->port = port;
};

void error(std::string msg) {
    std::cout << msg << std::endl;
};

void Connection::listen() {
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    int newsockfd;
    char buffer[512];
    // 5 connections tops for this one
    ::listen(sockfd,5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    bzero(buffer,512);

    int n;
    do { 
        n = read(newsockfd,buffer,512);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n", buffer);
        std::cout << n << " <- number" << std::endl;
        //int errorCode = write(newsockfd,"I got your message\n",19);
        //if (errorCode < 0) error("ERROR writing to socket");
    } while (n > 0);

    std::cout << "Closing connection now" << std::endl;
    ::shutdown(newsockfd, SHUT_RD);
    ::close(newsockfd);
    std::cout << "Closing server connection now" << std::endl;
    ::shutdown(newsockfd, SHUT_RDWR);
    ::close(this->sockfd);
};

void Connection::open() {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    
    // get file descriptor for socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if OS decided to not give a process socket
    if (sockfd < 0) 
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

void Connection::close() {

};