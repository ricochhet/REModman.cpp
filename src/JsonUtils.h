#ifndef REMODMAN_JSONUTILS_INCLUDED
#define REMODMAN_JSONUTILS_INCLUDED

#pragma once

#include <json.hpp>
#include <iostream>
#include <fstream>
#include <variant>

namespace JsonUtils
{
    nlohmann::json load_json(const std::string& path);
    std::string get_string_value(const std::string &path, const std::string &key);
    int get_integer_value(const std::string& path, const std::string& key);
    void write_json_to_file(const std::string& fileName, const nlohmann::json& data);
    void create_or_update_json(const std::string& path, const std::string& key, const std::variant<std::string, int>& value, bool update);
}

#endif REMODMAN_JSONUTILS_INCLUDED