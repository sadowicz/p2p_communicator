#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

#include <IOException.h>
#include <unordered_map>

#include <QDebug>

#define CONFIG_DEFAULT_FILE_PATH "config.txt"

#define CONFIG_DEFAULT_FILE_CONTENTS "log-file=log.txt\nhistory-log-file=history.json\nport=8080\ndownloads-directory=downloads\ndebug-mode=false\nconfig-file=" CONFIG_DEFAULT_FILE_PATH

using namespace std;

class Config {

public:
    static Config& config() {
        static Config configuration;
        return configuration;
    }

    static string& config(const char* key) {
        return Config::config()[key];
    }

    void save();

    bool debugMode() {return isDebugMode;}

    string& operator[](const char* key){
        return values[key];
    }

private:
    bool isDebugMode;

    string configFile;
    unordered_map<string, string> values;
    void writeDefaultConfig();
    bool shouldCreateDefaultConfig();
    void loadConfiguration();
    Config();

};


#endif // CONFIG_H
