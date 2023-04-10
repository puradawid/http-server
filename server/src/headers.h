#ifndef HEADERS_H
#define HEADERS_H

#include <string>
#include <list>
#include <map>

#include "serializable.h"

/// @brief represents a single header (name: value) with additional functions for alternating it.
class Header : Serializable {
private:
    std::string mName;
    std::list<std::string> mValues;
public:
    /// @brief creates an empty header
    Header();

    /// @brief creates a header with list of values
    /// @param name name of the header
    /// @param values list of values (ordering means the order of serializing and parsing in case of many headers of the same value)
    Header(std::string name, std::list<std::string> values);

    /// @brief a single-value header
    /// @param name name of the header 
    /// @param value value of the header
    Header(std::string name, std::string value);

    /// @brief changes values (replaces an existing list) by creating a new Header instance
    /// @param values list of values that are going to be replaced
    /// @return copy of header with replaced values
    Header changeValues(std::list<std::string> values);

    /// @brief changes values (actually replaces a list with values with single-item list) by creating a new Header instance
    /// @param value a value of the header
    /// @return copy of the instance with the provided value
    Header changeValue(std::string value);

    /// @brief returns a name of the header
    /// @return name of the header, can be empty if EMPTY_HEADER
    std::string name();

    /// @brief returns a first value (for some reason, a client code may not be interested or assume this is single-value header always)
    /// @return a value of the header, first item if multiple values exist
    std::string value();

    /// @brief returns a list of values
    /// @return copy of the value list
    std::list<std::string> values();

    std::string serialize();
};

const Header EMPTY_HEADER = Header("", "");

/// @brief represents headers of a message
class Headers : Serializable {
private:
    std::map<std::string, Header> mHeaders;
public:
    /// @brief creates empty collection of headers
    Headers();

    /// @brief creates a headers collection with some initial elements
    /// @param headers a list of headers
    Headers(std::list<Header> headers);

    /// @brief returns an existing header
    /// @param name name of the header (case insensitive!)
    /// @return Header found or EMPTY_HEADER
    Header find(std::string name);

    /// @brief adds new header, if the same exists - it adds a new value
    /// @param header header that is not EMPTY_HEADER
    void addHeader(Header header);

    /// @brief removes an existing header. Nothing happens if find(header) == EMPTY_HEADER
    /// @param header header to remove, despite the value provided
    void removeHeader(Header header);

    /// @brief Removes header
    /// @param name a name of the header (case insensitive)
    void removeHeader(std::string name);

    /// @brief checks whether the item is within the collection (case insensitive)
    /// @param name name of the header (case insensitive)
    /// @return true if the header exists in the collection (so that find returns not EMPTY_HEADER value), false otherwise
    bool contains(std::string name);

    /// @brief serializes headers to HTTP-like headers
    /// @return a string, new-line separated of headers, like "Header1: value\nHeader2: value2"
    std::string serialize();
};

#endif