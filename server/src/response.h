#ifndef RESPONSE_H
#define RESPONSE_H

#include "serializable.h"
#include "message.h"
#include <string>

/// @brief represents code of HTTP response
class ResponseCode {
private:
    short mCode;
    std::string mName;
public:
    /// @brief Creates a custom response code - this is expected to do!
    /// @param code code of the response, e.g. 200, 201, 503
    /// @param name name of the response, like "Not Found" or "OK" used in the response's preamble
    ResponseCode(int code, std::string name);

    /// @brief returns a code
    /// @return code, e.g. 200, 503
    int code();

    /// @brief returns a name of the reponse, this is used in the response's preamble
    /// @return name, like "Not Found"
    std::string name();
};

/// @brief already created, default responses, however user code may return another ones as well
namespace Responses {
    static ResponseCode RESP_200 = ResponseCode(200, "OK");
    static ResponseCode RESP_500 = ResponseCode(500, "Server Error");
};

/// @brief Response object that carries all necessary information
class Response : public Serializable, public Message {
private:
    ResponseCode& code;
public:
    /// @brief Creates a resposne with code
    /// @param code code of this response @see ResponseCode class
    Response(ResponseCode& code);

    /// @brief returns a response code
    /// @return response code
    ResponseCode getResponseCode();

    /// @brief serializes response to HTTP format
    /// @return formatted HTTP response with all data carried
    std::string serialize();
};

#endif