#ifndef RESPONSE_H
#define RESPONSE_H

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

const ResponseCode RESP_200 = ResponseCode(200, "OK");

class Response {
private:
    ResponseCode& code;
public:
    Response(ResponseCode& code);
    ResponseCode getResponseCode();
};

#endif