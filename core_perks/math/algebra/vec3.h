// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	class alignas(alignof(T) * 4) Vec3
	{
	public:
		CP_FORCE_INLINE Vec3() : x_(0), y_(0), z_(0) {}
		CP_FORCE_INLINE Vec3(T xyz) : x_(xyz), y_(xyz), z_(xyz) {}
		CP_FORCE_INLINE Vec3(T x, T y, T z) : x_(x), y_(y), z_(z) {}
		template <class U>
		CP_FORCE_INLINE Vec3(const Vec3<U>& o) : x_(static_cast<T>(o.x_)), y_(static_cast<T>(o.y_)), z_(static_cast<T>(o.z_)) {}

		CP_FORCE_INLINE T& operator[](int idx) { CP_ASSERT(idx < 3); return xyz_[idx]; }
		CP_FORCE_INLINE T operator[](int idx) const { CP_ASSERT(idx < 3); return xyz_[idx]; }

		CP_FORCE_INLINE T sum_elements() const { return x_ + y_ + z_; }
		CP_FORCE_INLINE T min_element() const { return min(min(x_, y_), z_); }
		CP_FORCE_INLINE T max_element() const { return max(max(x_, y_), z_); }

		CP_FORCE_INLINE T square_length() const { return dot(*this, *this); }
		CP_FORCE_INLINE T length() const requires std::floating_point<T> { return sqrt(square_length()); }

		CP_FORCE_INLINE bool is_normalized(float tolerance = 1e-3f) const { return abs(square_length() - 1) <= tolerance * tolerance; }

		CP_FORCE_INLINE bool all() const requires std::integral<T> { return x_ && y_ && z_; }
		CP_FORCE_INLINE bool any() const requires std::integral<T> { return x_ || y_ || z_; }

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

		CP_FORCE_INLINE Vec3& operator+=(const Vec3& v) { x_ += v.x_; y_ += v.y_; z_ += v.z_; return *this; }
		CP_FORCE_INLINE Vec3& operator-=(const Vec3& v) { x_ -= v.x_; y_ -= v.y_; z_ -= v.z_; return *this; }
		CP_FORCE_INLINE Vec3& operator*=(const Vec3& v) { x_ *= v.x_; y_ *= v.y_; z_ *= v.z_; return *this; }
		CP_FORCE_INLINE Vec3& operator/=(const Vec3& v) { x_ /= v.x_; y_ /= v.y_; z_ /= v.z_; return *this; }

		CP_FORCE_INLINE Vec3& operator+=(T s) { x_ += s; y_ += s; z_ += s; return *this; }
		CP_FORCE_INLINE Vec3& operator-=(T s) { x_ -= s; y_ -= s; z_ -= s; return *this; }
		CP_FORCE_INLINE Vec3& operator*=(T s) { x_ *= s; y_ *= s; z_ *= s; return *this; }
		CP_FORCE_INLINE Vec3& operator/=(T s) { x_ /= s; y_ /= s; z_ /= s; return *this; }

		CP_FORCE_INLINE Vec3 operator|(const Vec3& o) const requires std::integral<T> { return Vec3(x_ | o.x_, y_ | o.y_, z_ | o.z_); }
		CP_FORCE_INLINE Vec3 operator&(const Vec3& o) const requires std::integral<T> { return Vec3(x_ & o.x_, y_ & o.y_, z_ & o.z_); }
		CP_FORCE_INLINE Vec3& operator|=(const Vec3& o) requires std::integral<T> { *this = *this | o; return *this; }
		CP_FORCE_INLINE Vec3& operator&=(const Vec3& o) requires std::integral<T> { *this = *this & o; return *this; }

		CP_FORCE_INLINE static Vec3 zero() { return Vec3(0); }
		CP_FORCE_INLINE static Vec3 one() { return Vec3(1); }
		CP_FORCE_INLINE static Vec3 unit_x() { return Vec3(1, 0, 0); }
		CP_FORCE_INLINE static Vec3 unit_y() { return Vec3(0, 1, 0); }
		CP_FORCE_INLINE static Vec3 unit_z() { return Vec3(0, 0, 1); }
		CP_FORCE_INLINE static Vec3 left() { return unit_x(); }
		CP_FORCE_INLINE static Vec3 up() { return unit_y(); }
		CP_FORCE_INLINE static Vec3 forward() { return unit_z(); }

		union
		{
			struct { T x_, y_, z_; };
			T xyz_[3];
		};
	};

	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;
	using Vec3i = Vec3<int32>;
	using Vec3u = Vec3<uint32>;
	using Vec3b = Vec3<bool>;

	template <class T>
	CP_FORCE_INLINE Vec3<T> operator*(T a, const Vec3<T>& v)
	{
		return v * a;
	}

	template <class T>
	CP_FORCE_INLINE T dot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(a.y_ * b.z_ - a.z_ * b.y_, a.z_ * b.x_ - a.x_ * b.z_, a.x_ * b.y_ - a.y_ * b.x_);
	}

	template <std::floating_point T>
	CP_FORCE_INLINE Vec3<T> normalize(const Vec3<T>& a)
	{
		const T len = a.length();
		if (len > T(1e-5)) [[likely]]
			return a / len;
		else
			return Vec3<T>::zero();
	}

	template <class T>
	CP_FORCE_INLINE Vec3b equal(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3b(a.x_ == b.x_, a.y_ == b.y_, a.z_ == b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3b not_equal(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3b(a.x_ != b.x_, a.y_ != b.y_, a.z_ != b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3b less(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3b(a.x_ < b.x_, a.y_ < b.y_, a.z_ < b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3b less_or_equal(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3b(a.x_ <= b.x_, a.y_ <= b.y_, a.z_ <= b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3b greater(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3b(a.x_ > b.x_, a.y_ > b.y_, a.z_ > b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3b greater_or_equal(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3b(a.x_ >= b.x_, a.y_ >= b.y_, a.z_ >= b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> reflect(const Vec3<T>& incidence, const Vec3<T>& normal, T factor = T(1))
	{
		return incidence - normal * dot(normal, incidence) * (T(1) + factor);
	}
}
