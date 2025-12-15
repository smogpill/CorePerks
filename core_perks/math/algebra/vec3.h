// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	class alignas(16) Vec3
	{
	public:
		CP_FORCE_INLINE Vec3() = default;
		CP_FORCE_INLINE Vec3(T xyz) : x_(xyz), y_(xyz), z_(xyz) {}
		CP_FORCE_INLINE Vec3(T x, T y, T z) : x_(x), y_(y), z_(z) {}

		CP_FORCE_INLINE T square_length() const { return dot(*this, *this); }
		CP_FORCE_INLINE T length() const { return sqrt(square_length()); }
		CP_FORCE_INLINE T sum_elements() const { return x_ + y_ + z_; }
		CP_FORCE_INLINE T min_element() const { return min(min(x_, y_), z_); }
		CP_FORCE_INLINE T max_element() const { return max(max(x_, y_), z_); }

		CP_FORCE_INLINE T& operator[](int idx) { CP_ASSERT(idx < 3); return (&x_)[idx]; }
		CP_FORCE_INLINE T operator[](int idx) const { CP_ASSERT(idx < 3); return (&x_)[idx]; }

		CP_FORCE_INLINE bool operator==(const Vec3& v) const { return x_ == v.x_ && y_ == v.y_ && z_ == v.z_; }
		CP_FORCE_INLINE bool operator!=(const Vec3& v) const { return x_ != v.x_ || y_ != v.y_ || z_ != v.z_; }

		CP_FORCE_INLINE Vec3 operator-() const { return Vec3(-x_, -y_, -z_); } 
		CP_FORCE_INLINE Vec3 operator+(T s) const { return Vec3(x_ + s, y_ + s, z_ + s); }
		CP_FORCE_INLINE Vec3 operator-(T s) const { return Vec3(x_ - s, y_ - s, z_ - s); }
		CP_FORCE_INLINE Vec3 operator*(T s) const { return Vec3(x_ * s, y_ * s, z_ * s); }
		CP_FORCE_INLINE Vec3 operator/(T s) const { return Vec3(x_ / s, y_ / s, z_ / s); }
		CP_FORCE_INLINE Vec3 operator+(const Vec3& v) const { return Vec3(x_ + v.x_, y_ + v.y_, z_ + v.z_); }
		CP_FORCE_INLINE Vec3 operator-(const Vec3& v) const { return Vec3(x_ - v.x_, y_ - v.y_, z_ - v.z_); }
		CP_FORCE_INLINE Vec3 operator*(const Vec3& v) const { return Vec3(x_ * v.x_, y_ * v.y_, z_ * v.z_); }
		CP_FORCE_INLINE Vec3 operator/(const Vec3& v) const { return Vec3(x_ / v.x_, y_ / v.y_, z_ / v.z_); }

		CP_FORCE_INLINE Vec3& operator+=(T s) { x_ += s; y_ += s; z_ += s; return *this; }
		CP_FORCE_INLINE Vec3& operator-=(T s) { x_ -= s; y_ -= s; z_ -= s; return *this; }
		CP_FORCE_INLINE Vec3& operator*=(T s) { x_ *= s; y_ *= s; z_ *= s; return *this; }
		CP_FORCE_INLINE Vec3& operator/=(T s) { x_ /= s; y_ /= s; z_ /= s; return *this; }

		CP_FORCE_INLINE static Vec3 zero() { return Vec3(0); }
		CP_FORCE_INLINE static Vec3 one() { return Vec3(1); }
		CP_FORCE_INLINE static Vec3 unit_x() { return Vec3(1, 0, 0); }
		CP_FORCE_INLINE static Vec3 unit_y() { return Vec3(0, 1, 0); }
		CP_FORCE_INLINE static Vec3 unit_z() { return Vec3(0, 0, 1); }

		T x_ = 0;
		T y_ = 0;
		T z_ = 0;
	};

	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;
	using Vec3i = Vec3<int32>;

	template <class T>
	CP_FORCE_INLINE Vec3<T> operator*(T a, const Vec3<T>& v)
	{
		return v * a;
	}

	template <class T>
	CP_FORCE_INLINE T dot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.x_ * b._x + a.y_ * b.y_ + a.z_ * b.z_;
	}

	template <class T>
	CP_FORCE_INLINE T cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3(a.y_ * b.z_ - a.z_ * b.y_, a.z_ * b.x_ - a.x_ * b.z_, a.x_ * b.y_ - a.y_ * b.x_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> abs(const Vec3<T>& a)
	{
		return Vec3(abs(a.x_), abs(a.y_), abs(a.z_));
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> normalize(const Vec3<T>& a)
	{
		const T len = a.length();
		if (len > 1e-5f) [[likely]]
			return a / len;
		else
			return Vec3<T>::zero();
	}
}
