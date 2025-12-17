// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	using simd128 = __m128;

	template <class T> CP_FORCE_INLINE T fma(T x, T y, T z) { return std::fma(x, y, z); }
	template <class T> CP_FORCE_INLINE T abs(T x) { return std::abs(x); }
	template <class T> CP_FORCE_INLINE T sign(T x) { return (T(0) < x) - (x < T(0)); }
	template <class T> CP_FORCE_INLINE T copysign(T x, T y) { return std::copysign(x, y); }
	template <class T> CP_FORCE_INLINE T sqrt(T x) { return std::sqrt(x); }
	template <class T> CP_FORCE_INLINE T rsqrt(T x) { return T(1) / sqrt(x); }
	template <class T> CP_FORCE_INLINE T exp(T x) { return std::exp(x); }
	template <class T> CP_FORCE_INLINE T log(T x) { return std::log(x); }
	template <class T> CP_FORCE_INLINE T pow(T x, T y) { return std::pow(x, y); }
	template <class T> CP_FORCE_INLINE T exp2(T x) { return std::exp2(x); }
	template <class T> CP_FORCE_INLINE T log2(T x) { return std::log2(x); }
	template <class T> CP_FORCE_INLINE T floor(T x) { return std::floor(x); }
	template <class T> CP_FORCE_INLINE T ceil(T x) { return std::ceil(x); }
	template <class T> CP_FORCE_INLINE T round(T x) { return std::round(x); }
	template <class T> CP_FORCE_INLINE T trunc(T x) { return std::trunc(x); }
	template <class T> CP_FORCE_INLINE T fract(T x) { return x - floor(x); }
	template <class T> CP_FORCE_INLINE T min(T x, T y) { return std::min(x, y); }
	template <class T> CP_FORCE_INLINE T max(T x, T y) { return std::max(x, y); }
	template <class T> CP_FORCE_INLINE T clamp(T x, T low, T high) { return std::clamp(x, low, high); }
	template <class T> CP_FORCE_INLINE T saturate(T x) { return clamp(x, T(0), T(1)); }

	template <class T> CP_FORCE_INLINE T mod(T x, T y)
	{
		CP_ASSERT(std::isfinite(y));
		CP_ASSERT(y);
		return fma(-y, trunc(x / y), x);
	}

	template <class T> CP_FORCE_INLINE T wrap(T x, T low, T high)
	{
		CP_ASSERT(low < high);
		const T range = high - low;
		return x - range * floor((x - low) / range);
	}

	/// Requires testing
	/*
	CP_FORCE_INLINE T log2(float x)
	{
		uint32 c = std::bit_cast<uint32>(x);
		c = (c >> 23) - 127;
		return std::bit_cast<T>(c);
	}*/
}
