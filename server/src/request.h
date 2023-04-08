#ifndef REQUEST_H
#define REQUEST_H

#include <string>

#include "message.h"

enum Method {
    GET, POST, PUT, DELETE, OPTIONS
};

class Request : public Message {
private:
    Method mMethod;
    std::string mPath;
    Headers mHeaders;
public:
    Request();
    Request(std::string method);
    Request(Method method);
    Request(Method method, std::string path, Headers headers);
    Request(Method method, std::string path, Headers headers, std::string content);
    Method method();
    void path(std::string path);
    std::string path();
    std::string header(std::string name);
};

#endif