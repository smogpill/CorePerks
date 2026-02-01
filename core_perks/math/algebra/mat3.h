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
		Mat3() : x_(Vec3<T>::unit_x()), y_(Vec3<T>::unit_y()), z_(Vec3<T>::unit_z()) {}
		Mat3(T diag) : x_(diag, 0, 0), y_(0, diag, 0), z_(0, 0, diag) {}
		Mat3(const Vec3<T>& x, const Vec3<T>& y, const Vec3<T>& z) : x_(x), y_(y), z_(z) {}
		template <class U>
		Mat3(const Mat3<U>& o) : x_(static_cast<Vec3<T>>(o.x_)), y_(static_cast<Vec3<T>>(o.y_)), z_(static_cast<Vec3<T>>(o.z_)) {}

		CP_FORCE_INLINE Vec3<T>& operator[](int idx) { CP_ASSERT(idx < 3); return xyz_[idx]; }
		CP_FORCE_INLINE const Vec3<T>& operator[](int idx) const { CP_ASSERT(idx < 3); return xyz_[idx]; }
		CP_FORCE_INLINE Vec3<T> operator*(const Vec3<T>& v) const { return x_ * v.x_ + y_ * v.y_ + z_ * v.z_; }
		Mat3 operator*(const Mat3& o) const;
		CP_FORCE_INLINE Mat3& operator*=(const Mat3& o) { *this = *this * o; return *this; }

		union
		{
			struct { Vec3<T> x_, y_, z_; };
			Vec3<T> xyz_[3];
		};
	};

	using Mat3f = Mat3<float>;
	using Mat3d = Mat3<double>;

	template <class T>
	Mat3<T> Mat3<T>::operator*(const Mat3& o) const
	{
		return Mat3<T>((*this) * o.x_, (*this) * o.y_, (*this) * o.z_);
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
