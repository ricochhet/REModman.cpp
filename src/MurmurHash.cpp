#include <MurmurHash.h>

uint32_t murmurhash3(const char* key, const uint32_t seed)
{
    std::u16string u16_key;
    utf8::utf8to16(key, key + strlen(key), std::back_inserter(u16_key));

    uint32_t hash;
    MurmurHash3_x86_32(u16_key.data(), u16_key.length() * 2, seed, &hash);
    
    return hash;
}