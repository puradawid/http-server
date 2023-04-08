#ifndef MESSAGE_H
#define MESSAGE_H

#include "headers.h"

class Message {
protected:
    Headers mHeaders;
    std::string mContent;
    Message(Headers headers);
    Message(Headers headers, std::string content);
public:
    Headers& headers();
    std::string host();
    void host(std::string host);
    int contentLenght();
    void content(std::string content, std::string contentType);
    std::string content();
};

#endif