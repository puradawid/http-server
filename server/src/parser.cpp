#include "parser.h"
#include <string>

static const std::map<std::string, Method> methodStrings {
    { "GET", Method::GET },
    { "POST", Method::POST},
    { "PUT", Method::PUT }
};

MessageChunk::MessageChunk(std::string message)
{
    this->mMessage = message;
};

MessageChunk::MessageChunk(char *message, int length)
{
    this->mMessage = std::string(message, length);
}

std::string MessageChunk::message()
{
    return this->mMessage;
};

Request HttpParser::construct()
{
    Request r = this->mHeadersParser->build();
    return Request(this->mMethod, this->mPath, r.headers());
}

Continue HttpParser::read(MessageChunk chunk)
{
    if (this->done)
    {
        return this->mHeadersParser->digest(PreambleChunk(chunk.message(), this->mMethod, this->mPath));
    }
    else
    {
        this->done = true;
        std::string message = chunk.message();
        std::string firstLine = message.substr(0, message.find('\n'));
        std::string rest = message.substr(message.find('\n') + 1);
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

        if (rest == "\n\n") // end of request - no headers 
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
    this->mHeadersParser = new HeadersParser();
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

HeadersParser::HeadersParser() {
    this->mHeaders = Headers();
}

Request HeadersParser::construct()
{
    return Request(Method::GET, "", this->mHeaders);
}

Continue HeadersParser::read(PreambleChunk chunk)
{
    std::string left = this->mCache.restore(chunk.message());
    while (left != "\n") 
    {
        std::string headerInput = left.substr(0, left.find_first_of('\n'));
        std::smatch match;
        std::regex_match(headerInput, match, header);
        if (match.ready() && !match.empty()) {
            this->mHeaders.addHeader(Header(match[1].str(), match[2].str()));
            left = left.substr(left.find_first_of('\n') + 1);
            if (left.find('\n') > 0) // there is more to come - continue
            {
                this->mCache.store(left);
                return Continue(true);
            }
        } else {
            this->mCache.store(left);
            return Continue(true);
        }
    }

    if (left != "\n") {
        this->mCache.store(left);
        return Continue(true);
    }
    return Continue(false);
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
