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

#define CONFIG_DEFAULT_FILE_CONTENTS "history-log-file=history.json\nport=8080\ndownloads-directory=downloads\nconfig-file=" CONFIG_DEFAULT_FILE_PATH

using namespace std;

class Config {

public:
    static void init(string& filename);
    static void init();
    static string& get(string& key);
    static string& get(const char* key);

private:

    static string configFile;
    static unordered_map<string, string> values;
    static void writeDefaultConfig();
    static bool shouldCreateDefaultConfig();
    static void loadConfiguration();
    static void loadProperties();
    Config() = delete;
};


#endif // CONFIG_H
