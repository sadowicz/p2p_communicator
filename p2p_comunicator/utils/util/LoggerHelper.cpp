#include <util/LoggerHelper.h>

Logger util::getLogger() {
    return Logger(Config::config("port"), Config::config().debugMode());
}
