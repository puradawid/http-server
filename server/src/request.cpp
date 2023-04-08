#include "request.h"
#include <string>
#include <map>

Request::Request(std::string m) : Message(Headers()) {
    static const std::map<std::string, Method> methodStrings {
        { "GET", Method::GET },
        { "POST", Method::POST},
        { "PUT", Method::PUT }
    };
    this->mMethod = methodStrings.at(m);
}

Request::Request() : Message(Headers()), mMethod(Method::GET) {
    this->mMethod = Method::GET;
}

Request::Request(Method m) : Message(Headers()) {
    this->mMethod = m;
}

Request::Request(Method method, std::string path, Headers headers) 
    : Message(headers), mMethod(method), mPath(path), mHeaders(headers) { }

Request::Request(Method method, std::string path, Headers headers, std::string content)
    : Message(headers, content), mMethod(method), mPath(path), mHeaders(headers)
{
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
