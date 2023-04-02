#include <Json.h>

void log_json(const nlohmann::json &j)
{
    std::cout << j.dump(4) << std::endl;
}

nlohmann::json Json::load_json(const std::string &path)
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

void Json::write_json_to_file(const std::string& filename, const nlohmann::json& data)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        nlohmann::json j = data;
        file << j.dump(4) << std::endl;
        file.close();
    }
}

std::string Json::get_value(const std::string &path, const std::string &key)
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

nlohmann::json Json::store_mod_info_as_json(const std::vector<ModInfo> &vectors)
{
    nlohmann::json j;

    for (const auto &vec : vectors)
    {
        nlohmann::json j_struct;
        j_struct["Name"] = vec.Name;
        j_struct["Hash"] = vec.Hash;
        j_struct["LoadOrder"] = vec.LoadOrder;
        j_struct["IsEnabled"] = vec.IsEnabled;
        nlohmann::json j_struct_files;

        for (const auto &file : vec.Files)
        {
            nlohmann::json j_struct_file;
            j_struct_file["InstallPath"] = file.InstallPath;
            j_struct_file["SourcePath"] = file.SourcePath;
            j_struct_file["Hash"] = file.Hash;
            j_struct_files.push_back(j_struct_file);
        }

        j_struct["Files"] = j_struct_files;
        j.push_back(j_struct);
    }

    return j;
}

void Json::create_or_update_json(const std::string& path, const std::string& key, const std::variant<std::string, int>& value)
{
    std::ifstream fileIn(path);

    if (fileIn.good())
    {
        // If the file already exists, read the existing JSON
        nlohmann::json j;
        fileIn >> j;

        // Check if the key already exists in the JSON
        auto it = j.find(key);
        if (it == j.end())
        {
            // Add the new key-value pair to the existing JSON
            if (std::holds_alternative<std::string>(value))
            {
                j[key] = std::get<std::string>(value);
            }
            else if (std::holds_alternative<int>(value))
            {
                j[key] = std::get<int>(value);
            }

            // Write the modified JSON back to the file
            std::ofstream fileOut(path);
            fileOut << j.dump(4) << std::endl;
        }
        else
        {
            // The key already exists in the JSON
            std::cout << "Updating " << key << " in the JSON" << std::endl;
            if (std::holds_alternative<std::string>(value))
            {
                *it = std::get<std::string>(value);
            }
            else if (std::holds_alternative<int>(value))
            {
                *it = std::get<int>(value);
            }

            // Write the modified JSON back to the file
            std::ofstream fileOut(path);
            fileOut << j.dump(4) << std::endl;
        }
    }
    else
    {
        // If the file does not exist, create a new JSON with the key-value pair
        nlohmann::json j;
        if (std::holds_alternative<std::string>(value))
        {
            j[key] = std::get<std::string>(value);
        }
        else if (std::holds_alternative<int>(value))
        {
            j[key] = std::get<int>(value);
        }

        // Write the new JSON to the file
        std::ofstream fileOut(path);
        fileOut << j.dump(4) << std::endl;
    }
}