// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	template <class T>
	class alignas(alignof(Vec3<T>)) Mat3
	{
	public:
		Mat3() = default;
		Mat3(T diag) : x_(diag, 0, 0), y_(0, diag, 0), z_(0, 0, diag) {}
		Mat3(const Vec3<T>& x, const Vec3<T>& y, const Vec3<T>& z) : x_(x), y_(y), z_(z) {}
		template <class U>
		Mat3(const Mat3<U>& o) : x_(static_cast<Vec3<T>>(o.x_)), y_(static_cast<Vec3<T>>(o.y_)), z_(static_cast<Vec3<T>>(o.z_)) {}

		CP_FORCE_INLINE Vec3<T>& operator[](int idx) { CP_ASSERT(idx < 3); return (&x_)[idx]; }
		CP_FORCE_INLINE const Vec3<T>& operator[](int idx) const { CP_ASSERT(idx < 3); return (&x_)[idx]; }

		Vec3<T> x_ = Vec3<T>::unit_x();
		Vec3<T> y_ = Vec3<T>::unit_y();
		Vec3<T> z_ = Vec3<T>::unit_z();
	};

	using Mat3f = Mat3<float>;
	using Mat3d = Mat3<double>;

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
			Vec3<T>(1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy)),
			Vec3<T>(2 * (xy + wz), 1 - 2 * (xx + zz), 2 * (yz - wx)),
			Vec3<T>(2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (xx + yy))
		);
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
	Mat3<T> transpose(const Mat3<T>& m)
	{
		return Mat3<T>(
			Vec3<T>(m.x_.x_, m.y_.x_, m.z_.x_),
			Vec3<T>(m.x_.y_, m.y_.y_, m.z_.y_),
			Vec3<T>(m.x_.z_, m.y_.z_, m.z_.z_)
		);
	}
}
