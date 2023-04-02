#ifndef __JSON_H
#pragma once

#include <json.hpp>
#include <ModInfo.h>
#include <iostream>
#include <fstream>
#include <variant>

namespace Json
{
    nlohmann::json store_mod_info_as_json(const std::vector<ModInfo> &vectors);
    nlohmann::json load_json(const std::string& path);
    std::string get_value(const std::string& path, const std::string& key);
    void write_json_to_file(const std::string& filename, const nlohmann::json& data);
    void create_or_update_json(const std::string& path, const std::string& key, const std::variant<std::string, int>& value);
}

#endif __JSON_H