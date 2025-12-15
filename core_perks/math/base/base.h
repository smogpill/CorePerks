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
	template <class T> CP_FORCE_INLINE T floor(T x) { return std::floor(x); }
	template <class T> CP_FORCE_INLINE T ceil(T x) { return std::ceil(x); }
	template <class T> CP_FORCE_INLINE T round(T x) { return std::round(x); }
	template <class T> CP_FORCE_INLINE T trunc(T x) { return std::trunc(x); }
	template <class T> CP_FORCE_INLINE T fract(T x) { return x - floor(x); }
	template <class T> CP_FORCE_INLINE T min(T x) { return std::min(x); }
	template <class T> CP_FORCE_INLINE T max(T x) { return std::max(x); }
	template <class T> CP_FORCE_INLINE T clamp(T x, T low, T high) { return std::clamp(x, low, high); }

	template <class T> CP_FORCE_INLINE T mod(T x, T y)
	{
		CP_ASSERT(std::isfinite(y));
		CP_ASSERT(y);
		return fma(-y, trunc(x / y), x);
	}
}
