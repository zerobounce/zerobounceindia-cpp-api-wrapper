#ifndef UTILS_H
#define UTILS_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<typename T>
T getOrDefault(json j, std::string key, T defaultValue) {
    if (j.count(key) == 0 || j[key].is_null()) {
        return defaultValue;
    }
    return j[key].get<T>();
}

#endif