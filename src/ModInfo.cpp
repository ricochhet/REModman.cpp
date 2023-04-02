#include <ModInfo.h>

ModFile make_mod_file(std::string install_path, std::string source_path, uint32_t hash)
{
    return {install_path, source_path, hash};
}

ModInfo make_mod_info(std::string name, uint32_t hash, int load_order, std::string base_path, bool is_enabled, std::vector<ModFile> files)
{
    return {name, hash, load_order, base_path, is_enabled, files};
}