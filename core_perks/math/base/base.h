// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	using simd128 = __m128;

	template <class T> concept HasXY = requires(const T& t) { t.x_; t.y_; };
	template <class T> concept HasXYZ = requires(const T& t) { t.x_; t.y_; t.z_; };
	template <class T> concept HasXYZW = requires(const T& t) { t.x_; t.y_; t.z_; t.w_; };

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& a)
	{
		if constexpr (HasXYZW<T>)
			return T(elementwise(func, a.x_), elementwise(func, a.y_), elementwise(func, a.z_), elementwise(func, a.w_));
		else if constexpr (HasXYZ<T>)
			return T(elementwise(func, a.x_), elementwise(func, a.y_), elementwise(func, a.z_));
		else if constexpr (HasXY<T>)
			return T(elementwise(func, a.x_), elementwise(func, a.y_));
		else
			return func(a);
	}

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& a, const T& b)
	{
		if constexpr (HasXYZW<T>)
			return T(elementwise(func, a.x_, b.x_), elementwise(func, a.y_, b.y_), elementwise(func, a.z_, b.z_), elementwise(func, a.w_, b.w_));
		else if constexpr (HasXYZ<T>)
			return T(elementwise(func, a.x_, b.x_), elementwise(func, a.y_, b.y_), elementwise(func, a.z_, b.z_));
		else if constexpr (HasXY<T>)
			return T(elementwise(func, a.x_, b.x_), elementwise(func, a.y_, b.y_));
		else
			return func(a, b);
	}

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& a, const T& b, const T& c)
	{
		if constexpr (HasXYZW<T>)
			return T(elementwise(func, a.x_, b.x_, c.x_), elementwise(func, a.y_, b.y_, c.z_), elementwise(func, a.z_, b.z_, c.z_), elementwise(func, a.w_, b.w_, c.w_));
		else if constexpr (HasXYZ<T>)
			return T(elementwise(func, a.x_, b.x_, c.x_), elementwise(func, a.y_, b.y_, c.z_), elementwise(func, a.z_, b.z_, c.z_));
		else if constexpr (HasXY<T>)
			return T(elementwise(func, a.x_, b.x_, c.x_), elementwise(func, a.y_, b.y_, c.z_));
		else
			return func(a, b, c);
	}

	/*
	template <size_t N, typename Func, typename T>
	CP_FORCE_INLINE void apply_to_element(Func&& func, const T& in, T& out)
	{
		if constexpr (N == 0)
			return;

		apply_to_element<N - 1>(func, in, out);
		std::get<N - 1>(out) = func(std::get<N - 1>(in));
	}

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& value)
	{
		T result;
		apply_to_element<std::tuple_size<T>::value>(func, value, result);
		return result;
	}*/

	//CP_FORCE_INLINE
#define CP_ELEMENTWISE_1(impl) elementwise([](const auto& x) impl, x)
#define CP_ELEMENTWISE_2(impl) elementwise([](const auto& x, const auto& y) impl, a, b)
#define CP_ELEMENTWISE_3(impl) elementwise([](const auto& x, const auto& y, const auto& z) impl, a, b, c)

	template <class T> CP_FORCE_INLINE T abs(const T& x) { return CP_ELEMENTWISE_1({ return std::abs(x); }); }

	//template <class T> CP_FORCE_INLINE T abs(const T& x) { return elementwise([](const auto& v) { return std::abs(v); }, x); }

	template <class T> CP_FORCE_INLINE T fma(T x, T y, T z) { return std::fma(x, y, z); }
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
	template <class T> CP_FORCE_INLINE T min(const T& a, const T& b) { return elementwise([](const auto& x, const auto& y) { return std::min(x, y); }, a, b); }
	template <class T> CP_FORCE_INLINE T max(const T& a, const T& b) { return elementwise([](const auto& x, const auto& y) { return std::max(x, y); }, a, b); }
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
