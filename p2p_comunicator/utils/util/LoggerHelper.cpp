#include <util/LoggerHelper.h>

Logger util::getLogger() {
    return Logger(Config::config("log-file"), Config::config().debugMode());
}
