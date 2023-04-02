#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "request.h"
#include <regex>

struct Continue {
    bool continueProcessing;
    std::string errorMessage;
    Continue(bool continueProcessing) {
        this->continueProcessing = continueProcessing;
    }
    Continue(std::string message) {
        this->continueProcessing = false;
        this->errorMessage = message;
    }
};

class MessageChunk {
protected:
    std::string mMessage;
public:
    MessageChunk(char* message, int length);
    MessageChunk(std::string message);
    std::string message();
};

class PreambleChunk : public MessageChunk {
protected:
    std::string mPath;
    Method mMethod;
public:
    PreambleChunk(std::string message, Method method, std::string path);
    Method method();
    std::string path();
};

template <class Digest, class Result>
class Parser {
public:
    Continue digest(Digest d) {
        return this->read(d);
    }
    Result build() {
        return this->construct();
    }
protected:
    virtual Result construct() = 0;
    virtual Continue read(Digest d) = 0;
};

class HttpParser: public Parser<MessageChunk, Request> {
private:
    Method mMethod;
    std::regex preamble;
    std::string mPath;
    Request mRequest;
    bool done;
protected:
    Request construct();
    Continue read(MessageChunk chunk);
public:
    HttpParser();
};

#endif