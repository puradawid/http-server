#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>

/// @brief an interface that suggests the object can be transformed for HTTP consumption
class Serializable {
public:
    /// @brief returns a HTTP-consumable string
    /// @return string value that can be sent through any HTTP channel, like TCP
    virtual std::string serialize() = 0;
};

#endif