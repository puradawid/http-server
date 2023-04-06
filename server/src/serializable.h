#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

class Serializable {
public:
    virtual std::string serialize() = 0;
};

#endif