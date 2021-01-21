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

#define CONFIG_DEFAULT_FILE_CONTENTS "port=8080"

using namespace std;

class Config {

public:
    static void init(string filename = CONFIG_DEFAULT_FILE_PATH);
    static string get(string key);

private:
    static string configFile;
    static unordered_map<string, string> values;
    static void writeDefaultConfig();
    static bool shouldCreateDefaultConfig();
    static void loadConfiguration();
    Config() = delete;
};


#endif // CONFIG_H
