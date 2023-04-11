#ifndef REMODMAN_JSONUTILS_INCLUDED
#define REMODMAN_JSONUTILS_INCLUDED

#pragma once

#include <logger.h>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <variant>

namespace JsonUtils {
    nlohmann::json loadJson(const std::string& path);
    std::string    getString(const std::string& path, const std::vector<std::string>& keys);
    std::string    getString(const nlohmann::json& j, const std::vector<std::string>& keys);
    int            getInt(const std::string& path, const std::vector<std::string>& keys);
    int            getInt(const nlohmann::json& j, const std::vector<std::string>& keys);
    bool           getBool(const std::string& path, const std::vector<std::string>& keys);
    bool           getBool(const nlohmann::json& j, const std::vector<std::string>& keys);
    void           writeJson(const std::string& fileName, const nlohmann::json& data);
    void           writeJson(const std::string& path);
    void           updateJson(const std::string& path, const std::vector<std::string>& keys, const std::variant<std::string, int, bool, nlohmann::json>& value, bool update);
    nlohmann::json getJson(const std::string& path, const std::vector<std::string>& keys);
    nlohmann::json getJson(const nlohmann::json& j, const std::vector<std::string>& keys);
}

#endif  // REMODMAN_JSONUTILS_INCLUDED