#pragma once

#include <unordered_map>

namespace util {

template<typename K, typename V>
inline bool has(std::unordered_map<K, V> container, K& key) {
    return container.find(key) != container.end();
}
}
