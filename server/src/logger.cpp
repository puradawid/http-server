#include "logger.h"

#include <iostream>

using namespace Logging;

void StdoutLogger::write(std::string level, std::string message)
{
    std::cout << this->name << " [" << level << "]: " << message << std::endl;
}

StdoutLogger::StdoutLogger(std::string name) : name(name)
{
}

void StdoutLogger::debug(std::string message)
{
    this->write("debug", message);
}

void StdoutLogger::warn(std::string message)
{
    this->write("warn", message);
}

void StdoutLogger::error(std::string message)
{
    this->write("error", message);
}

Logger& StdoutLoggerFactory::getLogger(std::string name)
{
    return *(new StdoutLogger(name));
}