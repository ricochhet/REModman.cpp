#include <ModManagerData.h>

nlohmann::json ModManagerData::mod_to_json(const Mod& mod) {
    nlohmann::json j;
    j["SourcePath"] = mod.SourcePath;
    j["Files"]      = nlohmann::json::array();

    for (const auto& file : mod.Files) {
        nlohmann::json fileJson;
        fileJson["SourcePath"]  = file.SourcePath;
        fileJson["InstallPath"] = file.InstallPath;
        j["Files"].push_back(fileJson);
    }

    return j;
}

ModManagerData::Mod ModManagerData::mod_from_json(const nlohmann::json& j) {
    Mod mod;
    mod.SourcePath = j["SourcePath"];
    mod.Files.clear();

    for (const auto& fileJson : j["Files"]) {
        File file;
        file.SourcePath  = fileJson["SourcePath"];
        file.InstallPath = fileJson["InstallPath"];

        mod.Files.push_back(file);
    }

    return mod;
}

nlohmann::json ModManagerData::mods_to_json(const std::vector<ModManagerData::Mod>& mods) {
    nlohmann::json j;

    for (const auto& mod : mods) {
        j.push_back(mod_to_json(mod));
    }

    if (j.empty()) {
        return nlohmann::json::array();
    }

    return j;
}

std::vector<ModManagerData::Mod> ModManagerData::mods_from_json(const nlohmann::json& j) {
    std::vector<Mod> mods;

    for (const auto& mod : j) {
        mods.push_back(mod_from_json(mod));
    }

    return mods;
}