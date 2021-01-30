#include <Logger.h>

#define DEBUG "[DEBUG]"
#define INFO "[INFO]"
#define ERROR "[ERROR]"


void Logger::debug(std::string message) {
    write(message, DEBUG_TYPE);
}

void Logger::info(std::string message) {
    write(message, INFO_TYPE);
}

void Logger::error(std::string message) {
    write(message, ERROR_TYPE);
}

void Logger::write(std::string message, LogType type) {
    if (type == DEBUG_TYPE && !debugMode) {
        return;
    }
    std::ofstream file;
    file.open(filename, std::ios_base::app | std::ios_base::out);
    if (file.is_open()) {
        file << buildHeader(type) + message + "\n";
        file.close();
    } else {
        throw std::logic_error("Log file opening failed");
    }
}

std::string Logger::getTimestamp() {
    time_t t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "[%d-%m-%Y %H-%M-%S]");
    return oss.str();
}

std::string Logger::buildHeader(LogType type) {
    switch (type) {
        case ERROR_TYPE: return ERROR + Logger::getTimestamp() + ": ";
        case DEBUG_TYPE: return DEBUG + Logger::getTimestamp() + ": ";
        case INFO_TYPE: return INFO + Logger::getTimestamp() + ": ";
    }
}
