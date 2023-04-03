#include <JsonUtils.h>

void log_json(const nlohmann::json &j)
{
    std::cout << j.dump(4) << std::endl;
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
        std::cout << "Failed to load JSON file at path: " << path << std::endl;
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
}

std::string JsonUtils::get_string_value(const std::string &path, const std::string &key)
{
    nlohmann::json j = load_json(path);

    if (j.contains(key))
    {
        return j[key].get<std::string>();
    }
    else
    {
        std::cout << "Key not found in JSON file at path: " << path << std::endl;
        return "";
    }
}

int JsonUtils::get_integer_value(const std::string &path, const std::string &key)
{
    nlohmann::json j = load_json(path);

    if (j.contains(key))
    {
        return j[key].get<int>();
    }
    else
    {
        std::cout << "Key not found in JSON file at path: " << path << std::endl;
        return 0;
    }
}

void JsonUtils::create_or_update_json(const std::string &path, const std::string &key, const std::variant<std::string, int> &value, bool update)
{
    std::ifstream fileIn(path);

    if (fileIn.good())
    {
        nlohmann::json j;
        fileIn >> j;

        auto it = j.find(key);
        if (it == j.end())
        {
            if (std::holds_alternative<std::string>(value))
            {
                j[key] = std::get<std::string>(value);
            }
            else if (std::holds_alternative<int>(value))
            {
                j[key] = std::get<int>(value);
            }

            std::ofstream fileOut(path);
            fileOut << j.dump(4) << std::endl;
        }
        else
        {
            if (update)
            {
                std::cout << "Updating " << key << " in the JSON" << std::endl;
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
    }
    else
    {
        nlohmann::json j;
        if (std::holds_alternative<std::string>(value))
        {
            j[key] = std::get<std::string>(value);
        }
        else if (std::holds_alternative<int>(value))
        {
            j[key] = std::get<int>(value);
        }

        std::ofstream fileOut(path);
        fileOut << j.dump(4) << std::endl;
    }
}