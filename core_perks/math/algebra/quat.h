// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec4.h"
#include "core_perks/math/algebra/vec3.h"
#include "core_perks/math/algebra/mat3.h"
#include "core_perks/math/numerical/trigonometry.h"

namespace cp
{
	template <class T>
	class alignas(alignof(T) * 4) Quat
	{
	public:
		CP_FORCE_INLINE Quat() : x_(0), y_(0), z_(0), w_(1) {}
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
		CP_FORCE_INLINE static Quat rotation_x(T angle) { const T half_angle = angle * 0.5; return Quat(sin(half_angle), 0, 0, cos(half_angle)); }
		CP_FORCE_INLINE static Quat rotation_y(T angle) { const T half_angle = angle * 0.5; return Quat(0, sin(half_angle), 0, cos(half_angle)); }
		CP_FORCE_INLINE static Quat rotation_z(T angle) { const T half_angle = angle * 0.5; return Quat(0, 0, sin(half_angle), cos(half_angle)); }

		union
		{
			struct { T x_; T y_; T z_; T w_;};
			T xyzw_[4];
		};
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
		CP_ASSERT(axis.is_normalized());
		const T half_angle(angle * T(0.5));
		return Quat<T>(axis * sin(half_angle), cos(half_angle));
	}


	template <class T>
	Mat3<T> mat3_cast(const Quat<T>& q)
	{
		const T xx = q.x_ * q.x_;
		const T yy = q.y_ * q.y_;
		const T zz = q.z_ * q.z_;
		const T xy = q.x_ * q.y_;
		const T xz = q.x_ * q.z_;
		const T yz = q.y_ * q.z_;
		const T wx = q.w_ * q.x_;
		const T wy = q.w_ * q.y_;
		const T wz = q.w_ * q.z_;
		return Mat3<T>(
			Vec3<T>(1.0 - 2.0 * (yy + zz), 2.0 * (xy + wz), 2.0 * (xz - wy)),
			Vec3<T>(2.0 * (xy - wz), 1.0 - 2.0 * (xx + zz), 2.0 * (yz + wx)),
			Vec3<T>(2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (xx + yy))
		);
		/*
		return Mat3<T>(
			Vec3<T>(1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy)),
			Vec3<T>(2 * (xy + wz), 1 - 2 * (xx + zz), 2 * (yz - wx)),
			Vec3<T>(2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (xx + yy))
		);
		*/
	}

	template <class T>
	Quat<T> quat_cast(const Mat3<T>& m)
	{
		// From glm::quat_cast

		T four_x_squared_minus_1 = m[0][0] - m[1][1] - m[2][2];
		T four_y_squared_minus_1 = m[1][1] - m[0][0] - m[2][2];
		T four_z_squared_minus_1 = m[2][2] - m[0][0] - m[1][1];
		T four_w_squared_minus_1 = m[0][0] + m[1][1] + m[2][2];

		int biggest_index = 0;
		T four_biggest_squared_minus_1 = four_w_squared_minus_1;
		if (four_x_squared_minus_1 > four_biggest_squared_minus_1)
		{
			four_biggest_squared_minus_1 = four_x_squared_minus_1;
			biggest_index = 1;
		}
		if (four_y_squared_minus_1 > four_biggest_squared_minus_1)
		{
			four_biggest_squared_minus_1 = four_y_squared_minus_1;
			biggest_index = 2;
		}
		if (four_z_squared_minus_1 > four_biggest_squared_minus_1)
		{
			four_biggest_squared_minus_1 = four_z_squared_minus_1;
			biggest_index = 3;
		}

		T biggest_val = sqrt(four_biggest_squared_minus_1 + static_cast<T>(1)) * static_cast<T>(0.5);
		T mult = static_cast<T>(0.25) / biggest_val;

		switch (biggest_index)
		{
		case 0:
			return Quat<T>((m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult, biggest_val);
		case 1:
			return Quat<T>(biggest_val, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] - m[2][1]) * mult);
		case 2:
			return Quat<T>((m[0][1] + m[1][0]) * mult, biggest_val, (m[1][2] + m[2][1]) * mult, (m[2][0] - m[0][2]) * mult);
		case 3:
			return Quat<T>((m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggest_val, (m[0][1] - m[1][0]) * mult);
		default: // Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
			CP_ASSERT(false);
			return Quat<T>::identity();
		}
	}

	template <class T>
	CP_FORCE_INLINE Quat<T> quat_from_euler_angles(T pitch, T yaw, T roll)
	{
		const T hp = pitch * T(0.5);
		const T hy = yaw * T(0.5);
		const T hr = roll * T(0.5);

		const T sp = sin(hp), cp = cos(hp);
		const T sy = sin(hy), cy = cos(hy);
		const T sr = sin(hr), cr = cos(hr);

		return Quat<T>(
			sp * cy * cr + cp * sy * sr,
			cp * sy * cr - sp * cy * sr,
			cp * cy * sr + sp * sy * cr,
			cp * cy * cr - sp * sy * sr
		);
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> euler_angles_from_quat(const Quat<T>& q)
	{
		Vec3<T> angles;

		// Pitch
		const T sinp = T(2) * (q.w_ * q.x_ - q.y_ * q.z_);
		angles.x_ = asin(clamp(sinp, T(-1), T(1)));

		// Yaw
		const T siny_cosp = T(2) * (q.w_ * q.y_ + q.x_ * q.z_);
		const T cosy_cosp = T(1) - T(2) * (q.x_ * q.x_ + q.y_ * q.y_);
		angles.y_ = atan2(siny_cosp, cosy_cosp);

		// Roll
		const T sinr_cosp = T(2) * (q.w_ * q.z_ + q.x_ * q.y_);
		const T cosr_cosp = T(1) - T(2) * (q.x_ * q.x_ + q.z_ * q.z_);
		angles.z_ = atan2(sinr_cosp, cosr_cosp);

		return angles;
	}

	/*
	template <class T>
	CP_FORCE_INLINE Quat<T> from_forward_up(const Vec3<T>& forward, const Vec3<T>& up = Vec3<T>::up())
	{
		Vec3f f = normalize(forward);
		Vec3f u = normalize(up);
		Vec3f l = normalize(cross(u, f));
		u = cross(f, l);
		return from_basis(l, u, f);
	}
	*/
}
