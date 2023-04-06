#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Logging {

    class Logger {
    public:
        virtual void debug(std::string message) = 0;
        virtual void warn(std::string message) = 0;
        virtual void error(std::string message) = 0;
    };

    class LoggerFactory {
    public:
        virtual Logger& getLogger(std::string name) = 0;
    };

    class StdoutLogger : public Logger {
    private:
        std::string name;
        void write(std::string level, std::string message);
    public:
        StdoutLogger(std::string name);
        void debug(std::string message);
        void warn(std::string message);
        void error(std::string message);
    };

    class StdoutLoggerFactory : public LoggerFactory {
    public:
        Logger& getLogger(std::string name);
    };

    static StdoutLoggerFactory slf;
    static LoggerFactory& LOG = static_cast<LoggerFactory&>(slf);
}

#endif