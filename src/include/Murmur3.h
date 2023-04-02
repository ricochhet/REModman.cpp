#ifndef _RE_MURMUR3_H
#pragma once

#include <iostream>
#include <sstream>
#include <MurmurHash3.h>
#include <utf8.h>

std::wstring hash_to_utf16(uint32_t hash);
uint32_t murmurhash3(const char* key, const uint32_t seed);

#endif _RE_MURMUR3_H