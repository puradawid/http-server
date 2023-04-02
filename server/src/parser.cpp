#include "parser.h"
#include <string>

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
    return mRequest;
}

Continue HttpParser::read(MessageChunk chunk)
{
    if (this->done)
    {
        return Continue(false);
    }
    else
    {
        std::string message = chunk.message();
        std::string firstLine = message.substr(0, message.find('\n'));
        std::string rest = message.substr(message.find('\n'));
        std::smatch match;
        std::regex_match(firstLine, match, this->preamble);
        if (!match.ready() || match.empty())
        {
            return Continue("error processing");
        }

        mRequest = Request(match[1].str()).method();
        if (match.length() >= 2)
        {
            mRequest.path(match[2].str());
        }

        if (rest == "\n\n") // end of request - no headers 
        {
            return Continue(false);
        }
        return Continue(true);
    }
}
HttpParser::HttpParser()
{
    this->preamble = std::regex("^(GET|POST|PUT|OPTIONS) (/[^ ]*) HTTP/1.1$");
    this->done = false;
    this->mRequest = Request(Method::GET);
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
