#ifndef LOGGERCONFIG_H
#define LOGGERCONFIG_H

#include "logger.h"

namespace Logging {
    
    class ConfigEntry {
    private:
        std::string mRegex;
        std::string mLoggerFactoryName;
        std::string mLevel;
    public:
        ConfigEntry(std::string regex, std::string loggerFactoryName, std::string level);
        ConfigEntry(std::string entry);
        std::string regex();
        std::string loggerFactoryName();
        std::string level();
    };

    class Configuration {
    private:
        std::string mFilename;
    public:
        Configuration(std::string filename);
        Factories toFactories();
    };

    class LoggerLoader {
    public:
        static LoggerFactory& load();
    };

    static LoggerFactory& LOG = LoggerLoader::load();
}

#endif