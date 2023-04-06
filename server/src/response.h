#ifndef RESPONSE_H
#define RESPONSE_H

#include "serializable.h"
#include "message.h"
#include <string>

class ResponseCode {
private:
    short mCode;
    std::string mName;
public:
    ResponseCode(int code, std::string name);
    int code();
    std::string name();
};

namespace Responses {
    static ResponseCode RESP_200 = ResponseCode(200, "OK");
    static ResponseCode RESP_500 = ResponseCode(500, "Server Error");
};

class Response : public Serializable, public Message {
private:
    ResponseCode& code;
public:
    Response(ResponseCode& code);
    ResponseCode getResponseCode();
    std::string serialize();
};

#endif