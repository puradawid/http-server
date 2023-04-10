#include "logger.h"

#include <iostream>
#include <regex>
#include <memory>

using namespace Logging;

void BaseLogger::write(std::string level, std::string message)
{
    time_t now = time(0);
    std::string timeStr = asctime(gmtime(&now));
    timeStr.pop_back(); // a hack - asctime() always returns a string with an endline
    this->write(timeStr + ": " + this->name + " [" + level + "]: " + message + "\n");
}

void BaseLogger::debug(std::string message)
{
    if (level == DEBUG)
        this->write("debug", message);
}

void BaseLogger::warn(std::string message)
{
    if (level <= WARN)
        this->write("warn", message);
}

void BaseLogger::error(std::string message)
{
    if (level <= ERROR)
        this->write("error", message);
}

Logging::BaseLogger::BaseLogger(LogLevel level, std::string name) : level(level), name(name) { }

BaseLoggerFactory::BaseLoggerFactory(LogLevel level) : mLevel(level) { };

void Logging::StdoutLogger::write(std::string entry)
{
    std::cout << entry;
}

StdoutLogger::StdoutLogger(LogLevel level, std::string name) : BaseLogger(level, name) {}

Logger& StdoutLoggerFactory::getLogger(std::string name)
{
    return *(new StdoutLogger(this->mLevel, name));
}

StdoutLoggerFactory::StdoutLoggerFactory(LogLevel level): BaseLoggerFactory(level) {}; 

Logging::FileLoggerFactory::FileLoggerFactory(LogLevel level, std::string filename) : BaseLoggerFactory(level), filename(filename) { }

Logger &Logging::FileLoggerFactory::getLogger(std::string name)
{
    return *(new FileLogger(this->mLevel, name, this->filename));
}

void Logging::FileLogger::write(std::string entry)
{
    FILE* handler = fopen(this->filename.c_str(), "a+"); // open and close? well, multiple threads can access this, it's not a great solution
    // better to use some semaphore here, or some other critical section protection
    fprintf(handler, "%s", entry.c_str());
    fclose(handler);
}

Logging::FileLogger::FileLogger(LogLevel level, std::string name, std::string filename) : BaseLogger(level, name), filename(filename) {}

Logging::HybridLoggerFactory::HybridLoggerFactory(LogLevel level, 
    std::list<std::pair<std::regex, std::shared_ptr<LoggerFactory>>> factories, LoggerFactory &defaultFactory) : 
        BaseLoggerFactory(level), mFactories(factories), mDefaultFactory(defaultFactory)
{
}

Logger &Logging::HybridLoggerFactory::getLogger(std::string name)
{
    std::shared_ptr<Logger> ptr;

    if (this->mLoggers.find(name) != this->mLoggers.end()) {
        return *(mLoggers.at(name));
    }

    for(std::pair<std::regex, std::shared_ptr<LoggerFactory>> entry : this->mFactories) {
        std::smatch s;
        std::regex_match(name, s, entry.first);

        if (s.ready() && !s.empty()) {
            ptr = std::shared_ptr<Logger>(&(entry.second->getLogger(name)));
            this->mLoggers[name] = ptr;
            return *ptr;
        }
    }

    ptr = std::shared_ptr<Logger>(&(this->mDefaultFactory.getLogger(name)));
    this->mLoggers[name] = ptr;
    return *ptr;
}
