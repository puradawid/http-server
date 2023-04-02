#include "request.h"
#include <string>
#include <map>

Request::Request(std::string m) {
    static const std::map<std::string, Method> methodStrings {
        { "GET", Method::GET },
        { "POST", Method::POST},
        { "PUT", Method::PUT }
    };
    this->mMethod = methodStrings.at(m);
    this->mHeaders = Headers();
}

Request::Request() {
    this->mMethod = Method::GET;
}

Request::Request(Method m) {
    this->mMethod = m;
}

void Request::path(std::string path) {
    this->mPath = path;
}

std::string Request::path() {
    return this->mPath;
}

Method Request::method() {
    return this->mMethod;
}

std::string Request::header(std::string name) {
    if (this->mHeaders.contains(name))
    {
        return this->mHeaders.find(name).value();
    } else 
    {
        return "";
    }
}