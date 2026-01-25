// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/base/base.h"
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	template <class T>
	class alignas(alignof(T) * 4) Vec4
	{
	public:
		CP_FORCE_INLINE Vec4() = default;
		CP_FORCE_INLINE Vec4(T xyzw) : x_(xyzw), y_(xyzw), z_(xyzw), w_(xyzw) {}
		CP_FORCE_INLINE Vec4(T xyz, T w) : x_(xyz), y_(xyz), z_(xyz), w_(w) {}
		CP_FORCE_INLINE Vec4(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {}
		CP_FORCE_INLINE Vec4(const Vec3<T>& xyz, T w) : x_(xyz.x_), y_(xyz.y_), z_(xyz.z_), w_(w) {}
		template <class U>
		CP_FORCE_INLINE Vec4(const Vec4<U>& o) : x_(static_cast<T>(o.x_)), y_(static_cast<T>(o.y_)), z_(static_cast<T>(o.z_)), w_(static_cast<T>(o.w_)) {}

		CP_FORCE_INLINE const Vec3<T>& xyz() const { return reinterpret_cast<const Vec3<T>&>(*this); }
		CP_FORCE_INLINE T square_length() const { return dot(*this, *this); }
		CP_FORCE_INLINE T length() const requires std::floating_point<T> { return sqrt(square_length()); }
		CP_FORCE_INLINE T sum_elements() const { return x_ + y_ + z_ + w_; }
		CP_FORCE_INLINE T min_element() const { return min(min(x_, y_), min(z_, w_)); }
		CP_FORCE_INLINE T max_element() const { return max(max(x_, y_), max(z_, w_)); }

		CP_FORCE_INLINE bool all() const requires std::integral<T> { return x_ && y_ && z_ && w_; }
		CP_FORCE_INLINE bool any() const requires std::integral<T> { return x_ || y_ || z_ || w_; }

		CP_FORCE_INLINE T& operator[](int idx) { CP_ASSERT(idx < 4); return (&x_)[idx]; }
		CP_FORCE_INLINE T operator[](int idx) const { CP_ASSERT(idx < 4); return (&x_)[idx]; }

		CP_FORCE_INLINE bool operator==(const Vec4& v) const { return x_ == v.x_ && y_ == v.y_ && z_ == v.z_ && w_ == v.w_; }
		CP_FORCE_INLINE bool operator!=(const Vec4& v) const { return x_ != v.x_ || y_ != v.y_ || z_ != v.z_ || w_ != v.w_; }

		CP_FORCE_INLINE Vec4 operator-() const { return Vec4(-x_, -y_, -z_, -w_); }
		CP_FORCE_INLINE Vec4 operator+(T s) const { return Vec4(x_ + s, y_ + s, z_ + s, w_ + s); }
		CP_FORCE_INLINE Vec4 operator-(T s) const { return Vec4(x_ - s, y_ - s, z_ - s, w_ - s); }
		CP_FORCE_INLINE Vec4 operator*(T s) const { return Vec4(x_ * s, y_ * s, z_ * s, w_ * s); }
		CP_FORCE_INLINE Vec4 operator/(T s) const { return Vec4(x_ / s, y_ / s, z_ / s, w_ / s); }
		CP_FORCE_INLINE Vec4 operator+(const Vec4& v) const { return Vec4(x_ + v.x_, y_ + v.y_, z_ + v.z_, w_ + v.w_); }
		CP_FORCE_INLINE Vec4 operator-(const Vec4& v) const { return Vec4(x_ - v.x_, y_ - v.y_, z_ - v.z_, w_ - v.w_); }
		CP_FORCE_INLINE Vec4 operator*(const Vec4& v) const { return Vec4(x_ * v.x_, y_ * v.y_, z_ * v.z_, w_ * v.w_); }
		CP_FORCE_INLINE Vec4 operator/(const Vec4& v) const { return Vec4(x_ / v.x_, y_ / v.y_, z_ / v.z_, w_ / v.w_); }

		CP_FORCE_INLINE Vec4& operator+=(const Vec4& v) { x_ += v.x_; y_ += v.y_; z_ += v.z_; w_ += v.w_; return *this; }
		CP_FORCE_INLINE Vec4& operator-=(const Vec4& v) { x_ -= v.x_; y_ -= v.y_; z_ -= v.z_; w_ -= v.w_; return *this; }
		CP_FORCE_INLINE Vec4& operator*=(const Vec4& v) { x_ *= v.x_; y_ *= v.y_; z_ *= v.z_; w_ *= v.w_; return *this; }
		CP_FORCE_INLINE Vec4& operator/=(const Vec4& v) { x_ /= v.x_; y_ /= v.y_; z_ /= v.z_; w_ /= v.w_; return *this; }

		CP_FORCE_INLINE Vec4& operator+=(T s) { *this = *this + s; return *this; }
		CP_FORCE_INLINE Vec4& operator-=(T s) { *this = *this - s; return *this; }
		CP_FORCE_INLINE Vec4& operator*=(T s) { *this = *this * s; return *this; }
		CP_FORCE_INLINE Vec4& operator/=(T s) { *this = *this / s; return *this; }

		CP_FORCE_INLINE static Vec4 zero() { return Vec4(0); }
		CP_FORCE_INLINE static Vec4 one() { return Vec4(1); }
		CP_FORCE_INLINE static Vec4 unit_x() { return Vec4(1, 0, 0, 0); }
		CP_FORCE_INLINE static Vec4 unit_y() { return Vec4(0, 1, 0, 0); }
		CP_FORCE_INLINE static Vec4 unit_z() { return Vec4(0, 0, 1, 0); }
		CP_FORCE_INLINE static Vec4 unit_w() { return Vec4(0, 0, 0, 1); }

		T x_ = 0;
		T y_ = 0;
		T z_ = 0;
		T w_ = 0;
	};

	/*
	template <class T>
	struct CallOnElements<Vec4<T>>
	{
		template <class F>
		CP_FORCE_INLINE static Vec4<T> call(F func, const Vec4<T>& a) { return Vec4<T>(func(a.x_), func(a.y_), func(a.z_), func(a.w_)); }
		template <class F>
		CP_FORCE_INLINE static Vec4<T> call(F func, const Vec4<T>& a, const Vec4<T>& b) { return Vec4<T>(func(a.x_, b.x_), func(a.y_, b.y_), func(a.z_, b.z_), func(a.w_, b.w_)); }
		template <class F>
		CP_FORCE_INLINE static Vec4<T> call(F func, const Vec4<T>& a, const Vec4<T>& b, const Vec4<T>& c) { return Vec4<T>(func(a.x_, b.x_, c.x_), func(a.y_, b.y_, c.y_), func(a.z_, b.z_, c.z_), func(a.w_, b.w_, c.w_)); }
	};
	*/

	using Vec4f = Vec4<float>;
	using Vec4d = Vec4<double>;
	using Vec4i = Vec4<int32>;
	using Vec4u = Vec4<uint32>;

	template <class T>
	CP_FORCE_INLINE Vec4<T> operator*(T a, const Vec4<T>& v)
	{
		return v * a;
	}

	template <class T>
	CP_FORCE_INLINE T dot(const Vec4<T>& a, const Vec4<T>& b)
	{
		return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_ + a.w_ * b.w_;
	}

	template <class T>
	CP_FORCE_INLINE Vec4<T> abs(const Vec4<T>& a)
	{
		return Vec4(abs(a.x_), abs(a.y_), abs(a.z_), abs(a.w_));
	}

	template <std::floating_point T>
	CP_FORCE_INLINE Vec4<T> normalize(const Vec4<T>& a)
	{
		const T len = a.length();
		if (len > T(1e-5)) [[likely]]
			return a / len;
		else
			return Vec4<T>::zero();
	}
}
