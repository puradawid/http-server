#include "headers.h"

Headers::Headers()
{
    this->mHeaders = std::map<std::string, Header>();
}

Header Headers::find(std::string name)
{
    return this->mHeaders[name];
}

void Headers::addHeader(Header header)
{
    this->mHeaders[header.name()] = header;
}

void Headers::removeHeader(Header header)
{
    this->mHeaders.erase(header.name());
}

void Headers::removeHeader(std::string name)
{
    this->mHeaders.erase(name);
}

bool Headers::contains(std::string name)
{
    return this->mHeaders.find(name) != this->mHeaders.end();
}

Header::Header()
{
    this->mName = "";
    this->mValues = std::list<std::string>();
}

Header::Header(std::string name, std::list<std::string> values)
{
    this->mName = name;
    this->mValues = values;
}

Header::Header(std::string name, std::string value)
{
    this->mName = name;
    this->mValues = std::list<std::string>();
    this->mValues.push_back(value);
}

Header Header::changeValues(std::list<std::string>)
{
    return Header("Connection-Type", "allowed");
}

Header Header::changeValue(std::string value)
{
    return Header(this->mName, value);
}

std::string Header::name()
{
    return this->mName;
}

std::string Header::value()
{
    return this->mValues.front();
}

std::list<std::string> Header::values()
{
    return this->mValues;
}
