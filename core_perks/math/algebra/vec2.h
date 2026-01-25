// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T>
	class Vec2
	{
	public:
		CP_FORCE_INLINE Vec2() = default;
		CP_FORCE_INLINE Vec2(T xy) : x_(xy), y_(xy) {}
		CP_FORCE_INLINE Vec2(T x, T y) : x_(x), y_(y) {}
		template <class U>
		CP_FORCE_INLINE Vec2(const Vec2<U>& o) : x_(static_cast<T>(o.x_)), y_(static_cast<T>(o.y_)) {}

		CP_FORCE_INLINE T sum_elements() const { return x_ + y_; }
		CP_FORCE_INLINE T min_element() const { return min(x_, y_); }
		CP_FORCE_INLINE T max_element() const { return max(x_, y_); }

		CP_FORCE_INLINE T square_length() const { return dot(*this, *this); }
		CP_FORCE_INLINE T length() const requires std::floating_point<T> { return sqrt(square_length()); }

		CP_FORCE_INLINE bool all() const requires std::integral<T> { return x_ && y_; }
		CP_FORCE_INLINE bool any() const requires std::integral<T> { return x_ || y_; }

		CP_FORCE_INLINE T& operator[](int idx) { CP_ASSERT(idx < 2); return (&x_)[idx]; }
		CP_FORCE_INLINE T operator[](int idx) const { CP_ASSERT(idx < 2); return (&x_)[idx]; }

		CP_FORCE_INLINE bool operator==(const Vec2& v) const { return x_ == v.x_ && y_ == v.y_; }
		CP_FORCE_INLINE bool operator!=(const Vec2& v) const { return x_ != v.x_ || y_ != v.y_; }

		CP_FORCE_INLINE Vec2 operator-() const { return Vec2(-x_, -y_); }
		CP_FORCE_INLINE Vec2 operator+(T s) const { return Vec2(x_ + s, y_ + s); }
		CP_FORCE_INLINE Vec2 operator-(T s) const { return Vec2(x_ - s, y_ - s); }
		CP_FORCE_INLINE Vec2 operator*(T s) const { return Vec2(x_ * s, y_ * s); }
		CP_FORCE_INLINE Vec2 operator/(T s) const { return Vec2(x_ / s, y_ / s); }
		CP_FORCE_INLINE Vec2 operator+(const Vec2& v) const { return Vec2(x_ + v.x_, y_ + v.y_); }
		CP_FORCE_INLINE Vec2 operator-(const Vec2& v) const { return Vec2(x_ - v.x_, y_ - v.y_); }
		CP_FORCE_INLINE Vec2 operator*(const Vec2& v) const { return Vec2(x_ * v.x_, y_ * v.y_); }
		CP_FORCE_INLINE Vec2 operator/(const Vec2& v) const { return Vec2(x_ / v.x_, y_ / v.y_); }

		CP_FORCE_INLINE Vec2& operator+=(T s) { x_ += s; y_ += s; return *this; }
		CP_FORCE_INLINE Vec2& operator-=(T s) { x_ -= s; y_ -= s; return *this; }
		CP_FORCE_INLINE Vec2& operator*=(T s) { x_ *= s; y_ *= s; return *this; }
		CP_FORCE_INLINE Vec2& operator/=(T s) { x_ /= s; y_ /= s; return *this; }

		CP_FORCE_INLINE Vec2 operator|(const Vec2& o) const requires std::integral<T> { return Vec2(x_ | o.x_, y_ | o.y_); }
		CP_FORCE_INLINE Vec2 operator&(const Vec2& o) const requires std::integral<T> { return Vec2(x_ & o.x_, y_ & o.y_); }
		CP_FORCE_INLINE Vec2& operator|=(const Vec2& o) requires std::integral<T> { *this = *this | o; return *this; }
		CP_FORCE_INLINE Vec2& operator&=(const Vec2& o) requires std::integral<T> { *this = *this & o; return *this; }

		CP_FORCE_INLINE static Vec2 zero() { return Vec2(0); }
		CP_FORCE_INLINE static Vec2 one() { return Vec2(1); }
		CP_FORCE_INLINE static Vec2 unit_x() { return Vec2(1, 0); }
		CP_FORCE_INLINE static Vec2 unit_y() { return Vec2(0, 1); }

		T x_ = 0;
		T y_ = 0;
	};

	using Vec2f = Vec2<float>;
	using Vec2d = Vec2<double>;
	using Vec2i = Vec2<int32>;
	using Vec2u = Vec2<uint32>;
}
