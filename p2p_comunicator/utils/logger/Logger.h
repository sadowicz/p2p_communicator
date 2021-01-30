#pragma once

#include <config/Config.h>
#include <IOException.h>
#include <util/strbuilder.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

class Logger {

public:

    Logger() {
        filename = Config::config("log-file");
    }

    static Logger& log() {
        static Logger logger{};
        return logger;
    }

    void debug(std::string message);
    void info(std::string message);
    void error(std::string message);

    std::string getTimestamp();

private:
    std::string filename;

    enum LogType {
        DEBUG_TYPE, INFO_TYPE, ERROR_TYPE
    };

    std::string buildHeader(LogType type);
    void write(std::string message, LogType type);
    void writeLogHeader();
};
