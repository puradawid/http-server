#ifndef RESPONSE_H
#define RESPONSE_H

enum ResponseCode {
    OK200, BADREQUEST400, SERVER_ERROR500
};

class Response {
private:
    ResponseCode response;
public:
    Response(ResponseCode code);
    ResponseCode getResponse();
};

#endif