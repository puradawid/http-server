#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "request.h"
#include "serializable.h"
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

class Cache {
private:
    std::string mLastMsg;
public:
    Cache();
    void store(std::string message);
    std::string restore(std::string message);
};

class MessageChunk : public Serializable {
protected:
    std::string mMessage;
public:
    MessageChunk(char* message, int length);
    MessageChunk(std::string message);
    std::string message();
    std::string serialize();
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

class HeadersChunk : public PreambleChunk {
protected:
    Headers mHeaders;
public:
    HeadersChunk(PreambleChunk chunk, Headers headers);
    Headers headers();
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
    Parser() : done(false) {};
    bool done;
    virtual Result construct() = 0;
    virtual Continue read(Digest d) = 0;
};

class HttpParser: public Parser<MessageChunk, Request> {
private:
    Method mMethod;
    std::regex preamble;
    std::string mPath;
    Request mRequest;
    std::unique_ptr<Parser<PreambleChunk, Request>> mHeadersParser;
protected:
    Request construct();
    Continue read(MessageChunk chunk);
public:
    HttpParser();
};

class HeadersParser: public Parser<PreambleChunk, Request> {
private:
    Method method;
    std::string path;
    Headers mHeaders;
    Cache mCache;
    std::unique_ptr<Parser<HeadersChunk, Request>> mContentParser;
protected:
    Request construct();
    Continue read(PreambleChunk chunk);
public:
    HeadersParser();
};

class ContentParser: public Parser<HeadersChunk, Request> {
private:
    Method mMethod;
    std::string mPath;
    Headers mHeaders;
    std::string mContent;
    Cache mCache;
public:
    ContentParser();
    Request construct();
    Continue read(HeadersChunk chunk);
};
#endif