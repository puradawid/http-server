#include "parser.h"
#include <string>
#include <memory>

static const std::map<std::string, Method> methodStrings {
    { "GET", Method::GET },
    { "POST", Method::POST},
    { "PUT", Method::PUT }
};

static const std::string ENDL = "\r\n";

MessageChunk::MessageChunk(std::string message)
{
    this->mMessage = message;
};

MessageChunk::MessageChunk(char *message, int length) : MessageChunk(std::string(message, length))
{
}

std::string MessageChunk::message()
{
    return this->mMessage;
}

std::string MessageChunk::serialize()
{
    return this->mMessage;
};

Request HttpParser::construct()
{
    return this->mHeadersParser->build();
}

Continue HttpParser::read(MessageChunk chunk)
{
    if (chunk.message().length() == 0) {
        return Continue("empty message received");
    }
    if (this->done)
    {
        return this->mHeadersParser->digest(PreambleChunk(chunk.message(), this->mMethod, this->mPath));
    }
    else
    {
        this->done = true;
        std::string message = chunk.message();
        std::string firstLine = message.substr(0, message.find(ENDL));
        std::string rest = message.substr(message.find(ENDL) + 2);
        std::smatch match;
        std::regex_match(firstLine, match, this->preamble);
        if (!match.ready() || match.empty())
        {
            return Continue("error processing");
        }

        this->mMethod = methodStrings.at(match[1].str());
        if (match.length() >= 2)
        {
            mPath = match[2].str();
        }

        if (rest == ENDL + ENDL) // end of request - no headers 
        {
            return Continue(false);
        }

        return this->mHeadersParser->digest(PreambleChunk(rest, this->mMethod, this->mPath));
    }
}
HttpParser::HttpParser()
{
    this->preamble = std::regex("^(GET|POST|PUT|OPTIONS) (/[^ ]*) HTTP/1.1$");
    this->done = false;
    this->mRequest = Request(Method::GET);
    this->mHeadersParser = std::unique_ptr<HeadersParser>(new HeadersParser());
};

PreambleChunk::PreambleChunk(std::string message, Method method, std::string path) : MessageChunk(message)
{
    this->mMethod = method;
    this->mPath = path;
}

Method PreambleChunk::method()
{
    return this->mMethod;
}

std::string PreambleChunk::path()
{
    return this->mPath;
}

std::regex header = std::regex("^[ ]*([^ ]+)[ ]*:[ ]*([^ ].*+)$");

HeadersParser::HeadersParser() : method(Method::GET), path(""), mHeaders(Headers()) {
    this->mContentParser = std::unique_ptr<ContentParser>(new ContentParser());
}

Request HeadersParser::construct()
{
    return this->mContentParser->build();
}

Continue HeadersParser::read(PreambleChunk chunk)
{
    if (this->done) {
        return this->mContentParser->digest(HeadersChunk(chunk, this->mHeaders));
    }

    this->method = chunk.method();
    this->path = chunk.path();
    std::string left = this->mCache.restore(chunk.message());
    while (left != ENDL) 
    {
        std::string headerInput = left.substr(0, left.find(ENDL));
        std::smatch match;
        std::regex_match(headerInput, match, header);
        if (match.ready() && !match.empty()) {
            this->mHeaders.addHeader(Header(match[1].str(), match[2].str()));
            if (left.find(ENDL) != std::string::npos) {
                left = left.substr(left.find(ENDL) + 2);
            } else {
                this->mCache.store(left);
                return Continue(true);
            }
        } else {
            this->mCache.store(left);
            return Continue(true);
        }
    }

    if (left != ENDL) {
        this->mCache.store(left);
        return Continue(true);
    }
    this->done = true;
    return this->mContentParser->digest(HeadersChunk(chunk, this->mHeaders));
}


Cache::Cache()
{
    this->mLastMsg = "";
}

std::string Cache::restore(std::string msg) {
    std::string result = mLastMsg + msg;
    this->mLastMsg = "";
    return result;
}

void Cache::store(std::string msg) {
    this->mLastMsg = msg;
}

HeadersChunk::HeadersChunk(PreambleChunk chunk, Headers headers) : PreambleChunk(chunk), mHeaders(headers)
{
}

Headers HeadersChunk::headers()
{
    return this->mHeaders;
}

ContentParser::ContentParser() : mMethod(Method::GET), mPath(""), mContent("") {}

Request ContentParser::construct()
{
    return Request(this->mMethod, this->mPath, this->mHeaders, this->mContent);
}

Continue ContentParser::read(HeadersChunk chunk)
{
    this->mMethod = chunk.method();
    this->mPath = chunk.path();
    this->mHeaders = chunk.headers();

    return Continue(false);
}
