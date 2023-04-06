#include "response.h"

Response::Response(ResponseCode& code) : code(code) {};

ResponseCode Response::getResponseCode() {
    return this->code;
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
