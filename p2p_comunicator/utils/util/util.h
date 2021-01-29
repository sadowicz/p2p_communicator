#pragma once

#include <unordered_map>

#include <QString>

namespace util {

template<typename K, typename V>
inline bool has(std::unordered_map<K, V> container, K& key) {
    return container.find(key) != container.end();
}

template<typename K, typename V>
inline V& orElse(std::unordered_map<K, V> container, K& key, V& alternative) {
    return has(container, key)
        ? container[key]
        : alternative;
}

inline QString toQString(std::string& s) {
    return QString(s.c_str());
}
}
