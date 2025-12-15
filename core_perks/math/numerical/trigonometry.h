// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	CP_FORCE_INLINE constexpr T radians(T degrees) { return degrees * static_cast<T>(0.01745329251994329576923690768489); }

	template <class T>
	CP_FORCE_INLINE constexpr T degrees(T radians) { return radians * static_cast<T>(57.295779513082320876798154814105); }

	template <class T> CP_FORCE_INLINE T sin(T x) { return std::sin(x); }
	template <class T> CP_FORCE_INLINE T cos(T x) { return std::cos(x); }
	template <class T> CP_FORCE_INLINE T tan(T x) { return std::tan(x); }
	template <class T> CP_FORCE_INLINE T asin(T x) { return std::asin(x); }
	template <class T> CP_FORCE_INLINE T acos(T x) { return std::acos(x); }
	template <class T> CP_FORCE_INLINE T atan(T y, T x) { return std::atan2(y, x); }
	template <class T> CP_FORCE_INLINE T atan(T y_over_x) { return std::atan(y_over_x); }
}
