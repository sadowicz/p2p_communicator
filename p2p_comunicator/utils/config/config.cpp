#include "config.h"

string config::get(string key) {
    return config::values.at(key);
}

void config:;init(string configFile = config::configFile) {
    config::configFile = configFile;
    ifstream file(configFile);

    if (file.good()) {
        for (string line; getline(file, line);) {
            int index = line.find('=');
            string key = line.substr(0, index);
            string value = line.substr(index + 1, line.size());
            config::values.insert(key, value);
        }
    } else {
        throw new io_exception("Failed opening or creating the configuration file");
    }
    file.close();
}
