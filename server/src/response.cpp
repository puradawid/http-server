#include "response.h"

Response::Response(ResponseCode& code) : Message(Headers()), code(code) {};

ResponseCode Response::getResponseCode() {
    return this->code;
}

std::string Response::serialize()
{
    std::string message = "HTTP/1.1 " + std::to_string(this->code.code()) + " " 
        + this->code.name() + "\r\n" + this->mHeaders.serialize();
    if (this->mContent != "") {
        message += this->mContent;
    }
    return message;
};

ResponseCode::ResponseCode(int code, std::string name)
{
    this->mCode = code;
    this->mName = name;
}

int ResponseCode::code()
{
    return this->mCode;
}

std::string ResponseCode::name()
{
    return this->mName;
}
