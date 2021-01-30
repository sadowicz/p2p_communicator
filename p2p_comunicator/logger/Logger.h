#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <exception>
#include <stdexcept>

class Logger {

public:
    Logger() {}
    Logger(std::string& path, bool debugMode) : filename(path), debugMode(debugMode) {}

    void debug(std::string message);
    void info(std::string message);
    void error(std::string message);

    std::string getTimestamp();

private:
    std::string filename;
    bool debugMode;

    enum LogType {
        DEBUG_TYPE, INFO_TYPE, ERROR_TYPE
    };

    std::string buildHeader(LogType type);
    void write(std::string message, LogType type);
    void writeLogHeader();
};
