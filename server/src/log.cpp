#include "log.h"
#include <fstream>

using namespace Logging;
static const ConfigEntry EMPTY = ConfigEntry("", "", "");

Logging::ConfigEntry::ConfigEntry(std::string regex, std::string loggerFactoryName, std::string level) 
    : mRegex(regex), mLoggerFactoryName(loggerFactoryName), mLevel(level) { }

Logging::ConfigEntry::ConfigEntry(std::string entry)
{
    static const std::regex line = std::regex("^([^:]+):(file|stdout):([^:]+)$");
    std::smatch m;
    std::regex_match(entry, m, line);
    if (!m.ready() || m.empty()) {
        this->mRegex = "";
        this->mLoggerFactoryName = "";
        this->mLevel = "warn";
    } else {
        this->mRegex = m[1].str();
        this->mLoggerFactoryName = m[2].str();
        this->mLevel = m[3].str();
    }
}

std::string Logging::ConfigEntry::regex()
{
    return this->mRegex;
}

std::string Logging::ConfigEntry::loggerFactoryName()
{
    return this->mLoggerFactoryName;
}

std::string Logging::ConfigEntry::level()
{
    return this->mLevel;
}

LoggerFactory &Logging::LoggerLoader::load()
{
    LogLevel l = DEBUG;
    char *serverLoggerEnv = getenv("SERVER_LOGGER");
    if (serverLoggerEnv == NULL)
    {
        Configuration c("log.config");
        return *(new HybridLoggerFactory(LogLevel::WARN, c.toFactories(),
                                         *(new FileLoggerFactory(LogLevel::WARN, "warning.log"))));
    }
    std::string s(getenv("SERVER_LOGGER"));
    LoggerFactory *result = NULL;
    if (s == "file")
    {
        result = new FileLoggerFactory(l, "testing.log");
    }
    else if (s == "stdout")
    {
        result = new StdoutLoggerFactory(l);
    }
    return *(result);
}

Logging::Configuration::Configuration(std::string filename)
{
    this->mFilename = filename;
}

LogLevel parseLogLevel(std::string logLevel) {
    if (logLevel == "warn")
        return LogLevel::WARN;
    if (logLevel == "debug")
        return LogLevel::DEBUG;
    if (logLevel == "error")
        return LogLevel::ERROR;
    throw std::invalid_argument("Name of LogLevel unknown");
}

std::shared_ptr<LoggerFactory> parseLoggerFactory(std::string name, LogLevel level) {
    if (name == "file") {
        return std::make_shared<FileLoggerFactory>(level, "log.log");
    }
    if (name == "stdout") {
        return std::make_shared<StdoutLoggerFactory>(level);
    }
    throw std::invalid_argument("Name of LoggerFactory unknown");
} 

Factories Logging::Configuration::toFactories()
{
    Factories result;

    std::ifstream f(this->mFilename);

    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            ConfigEntry ce(line);
            result.push_back(Factory(ce.regex(), parseLoggerFactory(ce.loggerFactoryName(), parseLogLevel(ce.level()))));
        }
    }

    return result;
}
