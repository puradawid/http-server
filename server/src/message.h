#ifndef MESSAGE_H
#define MESSAGE_H

#include "headers.h"

/// @brief abstract class for messages that are having headers and content, these are base features of any HTTP-like request and
///        response
class Message {
protected:
    Headers mHeaders;
    std::string mContent;
    Message(Headers headers);
    Message(Headers headers, std::string content);
public:
    /// @brief returns a reference to headers. Modification is necessary for response, but forbudden for request
    /// @return headers collection reference
    Headers& headers();

    /// @brief returns host header (a shortcut for headers().find("host").value())
    /// @return Host value, like "test.com"
    std::string host();

    /// @brief sets up a host header
    /// @param host Host header value, like "test.com"
    void host(std::string host);

    /// @brief Number of characters in content for this specific message
    /// @return a content length
    int contentLenght();

    /// @brief sets up a content and its type
    /// @param content content of the message, like JSON object
    /// @param contentType a type existing in Content-Type header, like "application/json"
    void content(std::string content, std::string contentType);

    /// @brief returns content
    /// @return string of this message's content
    std::string content();
};

#endif