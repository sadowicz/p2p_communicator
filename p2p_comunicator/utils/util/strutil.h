#pragma once

#include <string>
#include <string.h>

namespace strutil {

inline bool startswith(char* str, char* prefix) {
    return strstr(str, prefix) == str;
}

inline bool startswith(const char* str, const char* prefix) {
    return strstr(str, prefix) == str;
}

inline bool endswith(char* str, char* suffix) {
    return strcmp(strrchr(str, suffix[0]), suffix) == 0;
}

inline bool endswith(const char* str, const char* suffix) {
    return strcmp(strrchr(str, suffix[0]), suffix) == 0;
}

inline bool contains(char* str, char* needle) {
    return strstr(str, needle);
}

inline bool contains(const char* str, const char* needle) {
    return strstr(str, needle);
}
}
