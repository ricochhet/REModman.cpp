#ifndef REMODMAN_MURMURHASH_INCLUDED
#define REMODMAN_MURMURHASH_INCLUDED

#pragma once

#include <MurmurHash3.h>
#include <utf8.h>

uint32_t murmurhash3(const char* key, const uint32_t seed);

#endif  // REMODMAN_MURMURHASH_INCLUDED