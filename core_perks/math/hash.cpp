// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/math/hash.h"

namespace cp::hash
{
	uint32 crc32::hash32(const void* data, uint64 length, uint32 seed)
	{
        const uint64* blocks = reinterpret_cast<const uint64*>(data);
        const uint64 num_blocks = length / 8;
        uint32 hash = seed;

        for (uint64 i = 0; i < num_blocks; ++i)
            hash = (uint32)_mm_crc32_u64(hash, blocks[i]);

        const uint8* bytes = reinterpret_cast<const uint8*>(data) + num_blocks * 8;
        for (uint64 i = 0; i < length % 8; ++i)
            hash = _mm_crc32_u8(hash, bytes[i]);

        return hash;
	}

#ifdef CP_XXHASH
    uint32 xxhash::hash32(const void* data, uint64 length, uint32 seed)
    {
        return XXH32(data, length, seed);
    }
#endif

#ifdef CP_XXHASH
    uint64 xxhash::hash64(const void* data, uint64 length, uint64 seed)
    {
        return XXH64(data, length, seed);
    }
#endif
}
