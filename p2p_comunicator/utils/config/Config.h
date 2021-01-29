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

#define CONFIG_DEFAULT_FILE_CONTENTS "history-log-file=history.json\nport=8080\ndownloads-directory=downloads\ndebug-mode=false\nconfig-file=" CONFIG_DEFAULT_FILE_PATH

using namespace std;

class Config {

public:
    static Config& config() {
        static Config configuration;
        return configuration;
    }

    string& get(string& key);
    string& get(const char* key);
    void set(string& key, string& value);
    void save();

private:
    string configFile;
    unordered_map<string, string> values;
    void writeDefaultConfig();
    bool shouldCreateDefaultConfig();
    void loadConfiguration();
    void loadProperties();
    Config();

};


#endif // CONFIG_H
