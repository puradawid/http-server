#include "response.h"

Response::Response(ResponseCode code) {
    this->response = code;
};

ResponseCode Response::getResponse() {
    return this->response;
};