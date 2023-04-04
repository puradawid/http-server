#ifndef REQUEST_H
#define REQUEST_H

#include <string>

#include "headers.h"

enum Method {
    GET, POST, PUT, DELETE, OPTIONS
};

class Request {
private:
    Method mMethod;
    std::string mPath;
    Headers mHeaders;
public:
    Request();
    Request(std::string method);
    Request(Method method);
    Request(Method method, std::string path, Headers headers);
    Method method();
    void path(std::string path);
    std::string path();
    std::string header(std::string name);
    Headers headers();
};

#endif