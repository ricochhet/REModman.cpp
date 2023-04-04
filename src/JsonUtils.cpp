#include <JsonUtils.h>

void log_json(const nlohmann::json &j)
{
    Logger::getInstance().log(j.dump(4), LogLevel::Info);
}

nlohmann::json JsonUtils::load_json(const std::string &path)
{
    std::ifstream fileIn(path);

    if (fileIn.good())
    {
        nlohmann::json j;
        fileIn >> j;
        return j;
    }
    else
    {
        Logger::getInstance().log("Failed to load JSON file at path: " + path, LogLevel::Error);
        return nlohmann::json();
    }
}

void JsonUtils::write_json_to_file(const std::string &fileName, const nlohmann::json &data)
{
    std::ofstream fileOut(fileName);
    if (fileOut.is_open())
    {
        nlohmann::json j = data;
        fileOut << j.dump(4) << std::endl;
        fileOut.close();
    }
    else
    {
        Logger::getInstance().log("Failed to write JSON file: " + fileName, LogLevel::Error);
    }
}

void JsonUtils::write_empty_json_to_file(const std::string &path)
{
    std::ifstream fileIn(path);

    if (!fileIn.good())
    {
        std::ofstream fileOut(path);
        nlohmann::json emptyArray = nlohmann::json::array();
        fileOut << emptyArray;
        fileOut.close();
    }
}

std::string JsonUtils::get_string_value(const std::string &path, const std::vector<std::string> &keys)
{
    nlohmann::json obj = load_json(path);
    for (const auto &key : keys)
    {
        if (obj.contains(key))
        {
            obj = obj[key];
        }
        else
        {
            Logger::getInstance().log("Key not found in JSON", LogLevel::Warning);
            return std::string();
        }
    }

    return obj.get<std::string>();
}

std::string JsonUtils::get_string_value(const nlohmann::json &j, const std::vector<std::string> &keys)
{
    nlohmann::json obj = j;
    for (const auto &key : keys)
    {
        if (obj.contains(key))
        {
            obj = obj[key];
        }
        else
        {
            Logger::getInstance().log("Key not found in JSON", LogLevel::Warning);
            return std::string();
        }
    }

    return obj.get<std::string>();
}

int JsonUtils::get_integer_value(const std::string &path, const std::vector<std::string> &keys)
{
    nlohmann::json obj = load_json(path);
    for (const auto &key : keys)
    {
        if (obj.contains(key))
        {
            obj = obj[key];
        }
        else
        {
            Logger::getInstance().log("Key not found in JSON", LogLevel::Warning);
            return 0;
        }
    }

    return obj.get<int>();
}

int JsonUtils::get_integer_value(const nlohmann::json &j, const std::vector<std::string> &keys)
{
    nlohmann::json obj = j;
    for (const auto &key : keys)
    {
        if (obj.contains(key))
        {
            obj = obj[key];
        }
        else
        {
            Logger::getInstance().log("Key not found in JSON", LogLevel::Warning);
            return 0;
        }
    }

    return obj.get<int>();
}

void JsonUtils::create_or_update_json(const std::string &path, const std::vector<std::string> &keys, const std::variant<std::string, int> &value, bool update)
{
    std::ifstream fileIn(path);

    if (fileIn.good())
    {
        nlohmann::json j;
        fileIn >> j;

        nlohmann::json *current = &j;
        for (const auto &key : keys)
        {
            auto it = current->find(key);
            if (it == current->end())
            {
                if (key == keys.back())
                {
                    if (std::holds_alternative<std::string>(value))
                    {
                        (*current)[key] = std::get<std::string>(value);
                    }
                    else if (std::holds_alternative<int>(value))
                    {
                        (*current)[key] = std::get<int>(value);
                    }

                    std::ofstream fileOut(path);
                    fileOut << j.dump(4) << std::endl;
                }
                else
                {
                    nlohmann::json newObject;
                    (*current)[key] = newObject;
                    current = &(*current)[key];
                }
            }
            else
            {
                if (key == keys.back())
                {
                    if (update)
                    {
                        Logger::getInstance().log("Updating " + key + " in JSON", LogLevel::Info);
                        if (std::holds_alternative<std::string>(value))
                        {
                            *it = std::get<std::string>(value);
                        }
                        else if (std::holds_alternative<int>(value))
                        {
                            *it = std::get<int>(value);
                        }

                        std::ofstream fileOut(path);
                        fileOut << j.dump(4) << std::endl;
                    }
                }
                else
                {
                    current = &(*it);
                }
            }
        }
    }
    else
    {
        nlohmann::json j;
        nlohmann::json *current = &j;
        for (const auto &key : keys)
        {
            if (key == keys.back())
            {
                if (std::holds_alternative<std::string>(value))
                {
                    (*current)[key] = std::get<std::string>(value);
                }
                else if (std::holds_alternative<int>(value))
                {
                    (*current)[key] = std::get<int>(value);
                }

                std::ofstream fileOut(path);
                fileOut << j.dump(4) << std::endl;
            }
            else
            {
                nlohmann::json newObject;
                (*current)[key] = newObject;
                current = &(*current)[key];
            }
        }
    }
}