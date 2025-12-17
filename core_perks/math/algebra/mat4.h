// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec4.h"

namespace cp
{
	template <class T>
	class alignas(alignof(Vec4<T>)) Mat4
	{
	public:
		Mat4() = default;
		Mat4(T diag) : x_(diag, 0, 0, 0), y_(0, diag, 0, 0), z_(0, 0, diag, 0), w_(0, 0, 0, diag) {}
		Mat4(const Vec4<T>& x, const Vec4<T>& y, const Vec4<T>& z, const Vec4<T>& w) : x_(x), y_(y), z_(z), w_(w) {}

		CP_FORCE_INLINE Vec4<T>& operator[](int idx) { CP_ASSERT(idx < 4); return (&x_)[idx]; }
		CP_FORCE_INLINE const Vec4<T>& operator[](int idx) const { CP_ASSERT(idx < 4); return (&x_)[idx]; }
		Mat4 operator*(const Mat4& o) const;
		CP_FORCE_INLINE Mat4& operator*=(const Mat4& o) const { *this = *this * o; return *this; }

	private:
		Vec4<T> x_ = Vec4<T>::unit_x();
		Vec4<T> y_ = Vec4<T>::unit_y();
		Vec4<T> z_ = Vec4<T>::unit_z();
		Vec4<T> w_ = Vec4<T>::unit_w();
	};

	using Mat4f = Mat4<float>;
	using Mat4d = Mat4<double>;

	template <class T>
	Mat4<T> Mat4<T>::operator*(const Mat4& o) const
	{
		const Vec4<T> ox = o.x_;
		const Vec4<T> oy = o.y_;
		const Vec4<T> oz = o.z_;
		const Vec4<T> ow = o.w_;
		return Mat4<T>(
			x_ * ox.x_ + y_ * ox.y_ + z_ * ox.z_ + w_ * ox.w_,
			x_ * oy.x_ + y_ * oy.y_ + z_ * oy.z_ + w_ * oy.w_,
			x_ * oz.x_ + y_ * oz.y_ + z_ * oz.z_ + w_ * oz.w_,
			x_ * ow.x_ + y_ * ow.y_ + z_ * ow.z_ + w_ * ow.w_);
	}

	template <class T>
	CP_FORCE_INLINE Mat4<T> operator*(T a, const Mat4<T>& m)
	{
		return Mat4<T>(a * m.x_, a * m.y_, a * m.z_, a * m.w_);
	}

	template <class T>
	CP_FORCE_INLINE Mat4<T> transpose(const Mat4<T>& m)
	{
		return Mat4<T>(
			Vec4<T>(m.x_.x_, m.y_.x_, m.z_.x_, m.w_.x_),
			Vec4<T>(m.x_.y_, m.y_.y_, m.z_.y_, m.w_.y_),
			Vec4<T>(m.x_.z_, m.y_.z_, m.z_.z_, m.w_.z_),
			Vec4<T>(m.x_.w_, m.y_.w_, m.z_.w_, m.w_.w_));
	}

	template <class T>
	CP_FORCE_INLINE Mat4<T> inverse(const Mat4<T>& m)
	{
		T coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		T coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		T coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		T coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		T coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		T coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		T coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		T coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		T coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		T coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		T coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		T coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		T coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		T coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		T coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		T coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		T coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		T coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vec4<T> fac0(coef00, coef00, coef02, coef03);
		Vec4<T> fac1(coef04, coef04, coef06, coef07);
		Vec4<T> fac2(coef08, coef08, coef10, coef11);
		Vec4<T> fac3(coef12, coef12, coef14, coef15);
		Vec4<T> fac4(coef16, coef16, coef18, coef19);
		Vec4<T> fac5(coef20, coef20, coef22, coef23);

		Vec4<T> vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Vec4<T> vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Vec4<T> vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Vec4<T> vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Vec4<T> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vec4<T> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vec4<T> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vec4<T> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vec4<T> sign_a(+1, -1, +1, -1);
		Vec4<T> sign_b(-1, +1, -1, +1);
		Mat4<T> inv(inv0 * sign_a, inv1 * sign_b, inv2 * sign_a, inv3 * sign_b);

		Vec4<T> row0(inv[0][0], inv[1][0], inv[2][0], inv[3][0]);

		Vec4<T> dot0(m[0] * row0);
		T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

		T one_over_determinant = static_cast<T>(1) / dot1;

		return inv * one_over_determinant;
	}
}
