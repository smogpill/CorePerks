// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/base/base.h"
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	template <class T>
	class alignas(16) Vec4
	{
	public:
		CP_FORCE_INLINE Vec4() = default;
		CP_FORCE_INLINE Vec4(T xyzw) : x_(xyzw), y_(xyzw), z_(xyzw), w_(xyzw) {}
		CP_FORCE_INLINE Vec4(T xyz, T w) : x_(xyz), y_(xyz), z_(xyz), w_(w) {}
		CP_FORCE_INLINE Vec4(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {}

		CP_FORCE_INLINE Vec3<T> xyz() const { return Vec3<T>(x_, y_, z_); }
		CP_FORCE_INLINE T square_length() const { return dot(*this, *this); }
		CP_FORCE_INLINE T length() const { return sqrt(square_length()); }
		CP_FORCE_INLINE T sum_elements() const { return x_ + y_ + z_ + w_; }
		CP_FORCE_INLINE T min_element() const { return min(min(x_, y_), min(z_, w_)); }
		CP_FORCE_INLINE T max_element() const { return max(max(x_, y_), max(z_, w_)); }

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

	using Vec4f = Vec4<float>;
	using Vec4d = Vec4<double>;
	using Vec4i = Vec4<int32>;

	template <class T>
	CP_FORCE_INLINE Vec4<T> operator*(T a, const Vec4<T>& v)
	{
		return v * a;
	}

	template <class T>
	CP_FORCE_INLINE T dot(const Vec4<T>& a, const Vec4<T>& b)
	{
		return a.x_ * b._x + a.y_ * b.y_ + a.z_ * b.z_ + a.w_ * b.w_;
	}

	template <class T>
	CP_FORCE_INLINE Vec4<T> abs(const Vec4<T>& a)
	{
		return Vec4(abs(a.x_), abs(a.y_), abs(a.z_), abs(a.w_));
	}

	template <class T>
	CP_FORCE_INLINE Vec4<T> normalize(const Vec4<T>& a)
	{
		const T len = a.length();
		if (len > 1e-5f) [[likely]]
			return a / len;
		else
			return Vec4<T>::zero();
	}
}
