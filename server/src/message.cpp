#include "message.h"

Message::Message(Headers headers) : mHeaders(headers)
{
}

Headers& Message::headers()
{
    return this->mHeaders;
}

std::string Message::host()
{
    return this->mHeaders.find("host").value();
}

void Message::host(std::string host)
{
    this->mHeaders.addHeader(Header("Host", host));
}

int Message::contentLenght()
{
    if (this->mHeaders.contains("Content-Length")) {
        return stoi(this->mHeaders.find("Content-Length").value());
    } else {
        return this->mContent.size();
    }
}

void Message::content(std::string content, std::string contentType)
{
    this->mContent = content;
    this->mHeaders.addHeader(Header("Content-Length", std::to_string(content.length())));
    this->mHeaders.addHeader(Header("Content-Type", contentType));
}
