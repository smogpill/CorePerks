// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/base/base.h"

namespace cp::hash
{
	namespace crc32
	{
		CP_FORCE_INLINE uint32 hash32(uint32 x, uint32 seed = 0) { return _mm_crc32_u32(seed, x); }
		CP_FORCE_INLINE uint64 hash64(uint64 x, uint64 seed = 0) { return _mm_crc32_u64(seed, x); }
		uint32 hash32(const void* data, uint64 length, uint32 seed = 0);
		inline uint32 hash32(const std::string& str, uint32 seed = 0) { return hash32(str.data(), str.length(), seed); }
	}
	
	namespace xxhash
	{
#ifdef CP_XXHASHCT
		inline constexpr uint32 hash32_compile_time(const void* data, uint32 length, uint32 seed = 0) { return xxh32::hash(reinterpret_cast<const char*>(data), length, seed); }
		constexpr uint32 hash32_compile_time(const std::string& str, uint32 seed = 0) { return hash32_compile_time(str.data(), (uint32)str.length(), seed); }

		constexpr uint64 hash64_compile_time(const void* data, uint32 length, uint64 seed = 0) { return xxh64::hash(reinterpret_cast<const char*>(data), length, seed); }
		constexpr uint64 hash64_compile_time(const std::string& str, uint64 seed = 0) { return hash64_compile_time(str.data(), (uint32)str.length(), seed); }
#endif
#ifdef CP_XXHASH
		uint32 hash32(const void* data, uint64 length, uint32 seed = 0);
		inline uint32 hash32(const std::string& str, uint32 seed = 0) { return hash32(str.data(), str.length(), seed); }

		uint64 hash64(const void* data, uint64 length, uint64 seed = 0);
		inline uint64 hash64(const std::string& str, uint64 seed = 0) { return hash64(str.data(), str.length(), seed); }
#endif
	}

	/*
#ifdef CP_XXHASHCT
	CP_FORCE_INLINE constexpr uint64 strong64_compile_time(const void* data, uint64 length, uint64 seed = 0) { return xxhash::hash64_compile_time(data, length, seed); }
	CP_FORCE_INLINE constexpr uint64 strong64_compile_time(const std::string& str, uint64 seed = 0) { return strong64_compile_time(str.data(), str.length(), seed); }
#endif
#ifdef CP_XXHASH
	CP_FORCE_INLINE uint64 strong64(const void* data, uint64 length, uint64 seed = 0) { return xxhash::hash64(data, length, seed); }
	CP_FORCE_INLINE uint64 strong64(const std::string& str, uint64 seed = 0) { return strong64(str.data(), str.length(), seed); }
#endif
*/
	namespace fast
	{
		using crc32::hash32;
		using crc32::hash64;
	}

	namespace strong
	{
#ifdef CP_XXHASH
		using xxhash::hash32;
		using xxhash::hash64;
#endif
#ifdef CP_XXHASHCT
		using xxhash::hash32_compile_time;
		using xxhash::hash64_compile_time;
#endif
	}
	//static auto& fast32 = crc32::hash32;
	//using fast64 = crc32::hash64;
	//CP_FORCE_INLINE uint32 fast32(uint32 x) { return crc32::hash32(x); }
	//CP_FORCE_INLINE uint64 fast64(uint64 x) { return crc32::hash64(x); }
}
