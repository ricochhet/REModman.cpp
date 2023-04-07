#ifndef REMODMAN_JSONUTILS_INCLUDED
#define REMODMAN_JSONUTILS_INCLUDED

#pragma once

#include <Logger.h>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <variant>

namespace JsonUtils {
    nlohmann::json load_json(const std::string& path);
    std::string    get_string_value(const std::string& path, const std::vector<std::string>& keys);
    std::string    get_string_value(const nlohmann::json& j, const std::vector<std::string>& keys);
    int            get_integer_value(const std::string& path, const std::vector<std::string>& keys);
    int            get_integer_value(const nlohmann::json& j, const std::vector<std::string>& keys);
    void           write_json_to_file(const std::string& fileName, const nlohmann::json& data);
    void           write_empty_json_to_file(const std::string& path);
    void           create_or_update_json(const std::string& path, const std::vector<std::string>& keys, const std::variant<std::string, int, nlohmann::json>& value, bool update);
    nlohmann::json get_json_value(const std::string& path, const std::vector<std::string>& keys);
    nlohmann::json get_json_value(const nlohmann::json& j, const std::vector<std::string>& keys);
}

#endif  // REMODMAN_JSONUTILS_INCLUDED