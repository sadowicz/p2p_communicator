#include <config/Config.h>

Config::Config() {
    configFile = CONFIG_DEFAULT_FILE_PATH;
    if (shouldCreateDefaultConfig()) {
        writeDefaultConfig();
    }

    loadConfiguration();
}

void Config::loadConfiguration() {
    ifstream file(configFile);
    if (file.is_open()) {
        for (string line; getline(file, line);) {
            int index = line.find('=');
            string key = line.substr(0, index);
            string value = line.substr(index + 1, line.size());
            values[key] = value;
        }
    } else {
        throw new IOException("Failed opening the configuration file");
    }
    isDebugMode = values["debug-mode"] == "true";
    file.close();
}

void Config::writeDefaultConfig() {
    ofstream file(configFile);
    if (!file.is_open()) {
        throw new IOException("Failed creating default configuration file");
    }
    file << CONFIG_DEFAULT_FILE_CONTENTS;
    file.close();
}

void Config::save() {
    ofstream file(configFile);
    if (!file.is_open()) {
        throw new IOException("Failed opening configuration file");
    }

    std::string res = "";

    for (std::pair<std::string, std::string> element : values) {
        res += element.first + "=" + element.second + "\n";
    }

    file << res;
    file.close();
}

bool Config::shouldCreateDefaultConfig() {
    ifstream file(Config::configFile);
    if (!file.is_open()) {
        return true;
    } else {
        file.close();
        return false;
    }
}
