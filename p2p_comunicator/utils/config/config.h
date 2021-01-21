#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

#include <io_exception.h>
#include <unordered_map>

using namespace std;

class config {

public:
    static void init(string configFile = config::configFile);
    static string get(string key);

private:
    static string configFile = "config.txt";
    static unordered_map<string, string> values;
    config() = delete;

};

#endif // CONFIG_H
