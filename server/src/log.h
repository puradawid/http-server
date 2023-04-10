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

    /// @brief The overall factory of logging that is the root of all logging code. Use this in the classes:
    /// @example static Logging::Logger LOG = Logging:LOG.getLogger("MyClassName");
    static LoggerFactory& LOG = LoggerLoader::load();
}

#endif