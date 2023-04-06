#ifndef MESSAGE_H
#define MESSAGE_H

#include "headers.h"

class Message {
protected:
    Headers mHeaders;
    Message(Headers headers);
    std::string mContent;
public:
    Headers& headers();
    std::string host();
    void host(std::string host);
    int contentLenght();
    void content(std::string content, std::string contentType);
};

#endif