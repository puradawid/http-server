#ifndef REQUEST_H
#define REQUEST_H

#include <string>

#include "message.h"

/// @brief Supported HTTP methods
enum Method {
    GET, POST, PUT, DELETE, OPTIONS
};

/// @brief a representation of HTTP request for handlers
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

    /// @brief Returns method used in request, like GET (see Method)
    /// @return method of that request 
    Method method();

    /// @brief sets up a path (used only for parsers)
    /// @param path a path of request, for example "/"
    void path(std::string path);

    /// @brief returns a path of this request
    /// @return path, like "/?q=123"
    std::string path();

    /// @brief gets a header value of specific name
    /// @param name a header name
    /// @returns that header's first value
    std::string header(std::string name);
};

#endif