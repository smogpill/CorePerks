// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec4.h"
#include "core_perks/math/algebra/vec3.h"
#include "core_perks/math/numerical/trigonometry.h"

namespace cp
{
	template <class T>
	class alignas(alignof(T) * 4) Quat
	{
	public:
		CP_FORCE_INLINE Quat() = default;
		CP_FORCE_INLINE Quat(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {}
		CP_FORCE_INLINE Quat(const Vec4<T>& v) : x_(v.x_), y_(v.y_), z_(v.z_), w_(v.w_) {}
		CP_FORCE_INLINE Quat(const Vec3<T>& xyz, T w) : x_(xyz.x_), y_(xyz.y_), z_(xyz.z_), w_(w) {}
		template <class U>
		CP_FORCE_INLINE Quat(const Quat<U>& o) : x_(static_cast<T>(o.x_)), y_(static_cast<T>(o.y_)), z_(static_cast<T>(o.z_)), w_(static_cast<T>(o.w_)) {}

		CP_FORCE_INLINE T square_length() const { return dot(*this, *this); }
		CP_FORCE_INLINE T length() const { return sqrt(square_length()); }
		T angle() const;
		Vec3<T> axis() const;

		CP_FORCE_INLINE bool operator==(const Quat& v) const { return x_ == v.x_ && y_ == v.y_ && z_ == v.z_ && w_ == v.w_; }
		CP_FORCE_INLINE bool operator!=(const Quat& v) const { return x_ != v.x_ || y_ != v.y_ || z_ != v.z_ || w_ != v.w_; }

		CP_FORCE_INLINE Quat operator-() const { return Quat(-x_, -y_, -z_, -w_); }
		CP_FORCE_INLINE Quat operator+(T s) const { return Quat(x_ + s, y_ + s, z_ + s, w_ + s); }
		CP_FORCE_INLINE Quat operator-(T s) const { return Quat(x_ - s, y_ - s, z_ - s, w_ - s); }
		CP_FORCE_INLINE Quat operator*(T s) const { return Quat(x_ * s, y_ * s, z_ * s, w_ * s); }
		CP_FORCE_INLINE Quat operator/(T s) const { return Quat(x_ / s, y_ / s, z_ / s, w_ / s); }
		CP_FORCE_INLINE Quat operator+(const Quat& v) const { return Quat(x_ + v.x_, y_ + v.y_, z_ + v.z_, w_ + v.w_); }
		CP_FORCE_INLINE Quat operator-(const Quat& v) const { return Quat(x_ - v.x_, y_ - v.y_, z_ - v.z_, w_ - v.w_); }

		CP_FORCE_INLINE Quat& operator+=(T s) { *this = *this + s; return *this; }
		CP_FORCE_INLINE Quat& operator-=(T s) { *this = *this - s; return *this; }
		CP_FORCE_INLINE Quat& operator*=(T s) { *this = *this * s; return *this; }
		CP_FORCE_INLINE Quat& operator/=(T s) { *this = *this / s; return *this; }

		CP_FORCE_INLINE static Quat identity() { return Quat(); }
		CP_FORCE_INLINE static Quat rotation_x(T angle) { const T half_angle = angle * 0.5; return Quat(sin(angle), 0, 0, cos(angle)); }
		CP_FORCE_INLINE static Quat rotation_y(T angle) { const T half_angle = angle * 0.5; return Quat(0, sin(angle), 0, cos(angle)); }
		CP_FORCE_INLINE static Quat rotation_z(T angle) { const T half_angle = angle * 0.5; return Quat(0, 0, sin(angle), cos(angle)); }

		T x_ = 0;
		T y_ = 0;
		T z_ = 0;
		T w_ = 1;
	};

	using Quatf = Quat<float>;
	using Quatd = Quat<double>;

	template <class T>
	CP_FORCE_INLINE T dot(const Quat<T>& a, const Quat<T>& b)
	{
		return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_ + a.w_ * b.w_;
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> normalize(const Quat<T>& a)
	{
		const T len = a.length();
		if (len > 1e-5f) [[likely]]
			return a / len;
		else
			return Quat<T>::identity();
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> conjugate(const Quat<T>& a)
	{
		return Quat(-a.x_ , -a.y_, -a.z_, a.w_);
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> inverse(const Quat<T>& a)
	{
		CP_ASSERT(a.square_length() - T(1) < T(1e-5)); // only for unit quaternions
		return conjugate(a);
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> operator*(T a, const Quat<T>& q)
	{
		return q * a;
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> operator*(const Quat<T>& a, const Quat<T>& b)
	{
		return Quat<T>(
			a.w_ * b.x_ + a.x_ * b.w_ + a.y_ * b.z_ - a.z_ * b.y_,
			a.w_ * b.y_ + a.y_ * b.w_ + a.z_ * b.x_ - a.x_ * b.z_,
			a.w_ * b.z_ + a.z_ * b.w_ + a.x_ * b.y_ - a.y_ * b.x_,
			a.w_ * b.w_ - a.x_ * b.x_ - a.y_ * b.y_ - a.z_ * b.z_);
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> operator*(const Quat<T>& q, const Vec3<T>& b)
	{
		// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/

		const Vec3<T> qxyz = std::bit_cast<Vec3<T>>(q);
		const Vec3<T> t = T(2) * cross(qxyz, b);
		return b + q.w_ * t + cross(qxyz, t);
	}

	template <class T>
	CP_FORCE_INLINE T Quat<T>::angle() const
	{
		if (abs(w_) > cos_one_over_two<T>())
		{
			const T a = asin(sqrt(x_ * x_ + y_ * y_ + z_ * z_)) * T(2);
			if (w_ < T(0))
				return pi<T>() * T(2) - a;
			return a;
		}

		return acos(w_) * T(2);
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> Quat<T>::axis() const
	{
		const T tmp1 = T(1) - w_ * w_;
		if (tmp1 <= T(0))
			return Vec3(0, 0, 1);
		const T tmp2 = T(1) / sqrt(tmp1);
		return *this * tmp2;
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> axis_angle(const Vec3<T>& axis, const T angle)
	{
		const T a(angle);
		const T s = sin(a * T(0.5));
		return Quat<T>(axis * s, cos(a * T(0.5)));
	}
}
