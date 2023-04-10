#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <regex>
#include <memory>
#include <list>

namespace Logging {

    enum LogLevel {
        DEBUG, WARN, ERROR
    };

    class Logger {
    public:
        virtual void debug(std::string message) = 0;
        virtual void warn(std::string message) = 0;
        virtual void error(std::string message) = 0;
    };

    /// @brief abstract class of basic logger - responsible for formatting and sending that information to abstrac write() method
    class BaseLogger : public Logger {
    private:
        LogLevel level;
        std::string name;
        void write(std::string level, std::string message);
    protected:
        BaseLogger(LogLevel level, std::string name);
        virtual void write(std::string entry) = 0;
    public:
        void debug(std::string message);
        void warn(std::string message);
        void error(std::string message);
    };

    class LoggerFactory {
    public:
        virtual Logger& getLogger(std::string name) = 0;
    };

    class BaseLoggerFactory : public LoggerFactory {
    protected:
        LogLevel mLevel;
    public:
        BaseLoggerFactory(LogLevel level);
    };

    class StdoutLogger : public BaseLogger {
    protected:
        void write(std::string entry);
    public:
        StdoutLogger(LogLevel level, std::string name);
    };

    class StdoutLoggerFactory : public BaseLoggerFactory {
    public:
        StdoutLoggerFactory(LogLevel level);
        Logger& getLogger(std::string name);
    };

    class FileLogger : public BaseLogger {
    private:
        std::string filename;
    protected:
        void write(std::string entry);
    public:
        FileLogger(LogLevel level, std::string name, std::string filename);
    };

    class FileLoggerFactory : public BaseLoggerFactory {
    private:
        std::string filename;
    public:
        FileLoggerFactory(LogLevel level, std::string filename);
        Logger& getLogger(std::string name);
    };
    
    typedef std::pair<std::regex, std::shared_ptr<LoggerFactory>> Factory;
    typedef std::list<Factory> Factories;

    class HybridLoggerFactory : public BaseLoggerFactory {
    private:
        std::map<std::string, std::shared_ptr<Logger>> mLoggers;
        Factories mFactories;
        LoggerFactory& mDefaultFactory;
    public:
        HybridLoggerFactory(LogLevel level, std::list<std::pair<std::regex, std::shared_ptr<LoggerFactory>>> factories, LoggerFactory& defaultFactory);
        Logger& getLogger(std::string name);
    };
}

#endif