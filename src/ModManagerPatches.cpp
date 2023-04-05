#include <ModManagerPatches.h>

ModManagerPatches::MonsterHunterRise::PatchReEnginePak ModManagerPatches::MonsterHunterRise::patch_re_engine_pak(const std::string &path, const std::string &modPath)
{
    nlohmann::json j = JsonUtils::load_json(path + "/profile.json");
    int pakModIndex = j["Patches"]["MonsterHunterRise"]["PatchReEnginePakIndex"];
    bool isPakMod = false;

    for (const auto &fileEntry : std::filesystem::directory_iterator(modPath))
    {
        if (fileEntry.is_regular_file())
        {
            std::string fileName = fileEntry.path().filename().string();

            if (fileName.substr(fileName.find_last_of(".") + 1) == "pak")
            {
                isPakMod = true;
                pakModIndex++;
                JsonUtils::create_or_update_json(path + "/profile.json", {"Patches", "MonsterHunterRise", "PatchReEnginePakIndex"}, pakModIndex, true);
                break;
            }
        }
    }

    ModManagerPatches::MonsterHunterRise::PatchReEnginePak pakInfo;
    pakInfo.isPak = isPakMod;
    pakInfo.pakIndex = pakModIndex;
    return pakInfo;
}