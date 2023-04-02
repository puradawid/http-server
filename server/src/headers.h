#ifndef HEADERS_H
#define HEADERS_H

#include <string>
#include <list>
#include <map>

class Header {
private:
    std::string mName;
    std::list<std::string> mValues;
public:
    Header();
    Header(std::string name, std::list<std::string> values);
    Header(std::string name, std::string value);
    Header changeValues(std::list<std::string> values);
    Header changeValue(std::string value);
    std::string name();
    std::string value();
    std::list<std::string> values();
};

class Headers {
private:
    std::map<std::string, Header> mHeaders;
public:
    Headers();
    Header find(std::string name);
    void addHeader(Header header);
    void removeHeader(Header header);
    void removeHeader(std::string name);
    bool contains(std::string name);
};

#endif