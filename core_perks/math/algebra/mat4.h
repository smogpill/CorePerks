// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec4.h"
#include "core_perks/math/algebra/mat3.h"
#include "core_perks/math/algebra/transform.h"

namespace cp
{
	template <class T>
	class alignas(alignof(Vec4<T>)) Mat4
	{
	public:
		Mat4() : x_(Vec4<T>::unit_x()), y_(Vec4<T>::unit_y()), z_(Vec4<T>::unit_z()), w_(Vec4<T>::unit_w()) {}
		Mat4(T diag) : x_(diag, 0, 0, 0), y_(0, diag, 0, 0), z_(0, 0, diag, 0), w_(0, 0, 0, diag) {}
		Mat4(const Vec4<T>& x, const Vec4<T>& y, const Vec4<T>& z, const Vec4<T>& w) : x_(x), y_(y), z_(z), w_(w) {}
		template <class U>
		Mat4(const Mat4<U>& o) : x_(static_cast<Vec4<T>>(o.x_)), y_(static_cast<Vec4<T>>(o.y_)), z_(static_cast<Vec4<T>>(o.z_)), w_(static_cast<Vec4<T>>(o.w_)) {}

		bool is_scale_uniform(float tolerance = 1e-3f) const;

		CP_FORCE_INLINE Vec4<T>& operator[](int idx) { CP_ASSERT(idx < 4); return xyzw_[idx]; }
		CP_FORCE_INLINE const Vec4<T>& operator[](int idx) const { CP_ASSERT(idx < 4); return xyzw_[idx]; }
		CP_FORCE_INLINE Vec4<T> operator*(const Vec4<T>& v) const { return x_ * v.x_ + y_ * v.y_ + z_ * v.z_ + w_ * v.w_; }
		CP_FORCE_INLINE Mat4<T> operator*(T s) const { return Mat4<T>(x_ * s, y_ * s, z_ * s, w_ * s); }
		Mat4 operator*(const Mat4& o) const;
		CP_FORCE_INLINE Mat4& operator*=(const Mat4& o) { *this = *this * o; return *this; }

		union
		{
			struct { Vec4<T> x_, y_, z_, w_; };
			Vec4<T> xyzw_[4];
		};
	};

	using Mat4f = Mat4<float>;
	using Mat4d = Mat4<double>;

	template<class T>
	CP_FORCE_INLINE bool Mat4<T>::is_scale_uniform(float tolerance) const
	{
		// Tolerance handling is a bit shady, but should be fine enough for now
		const float tolerance_sq = tolerance * tolerance;
		const T slx = x_.square_length();
		const T sly = y_.square_length();
		const T slz = z_.square_length();
		return abs(slx - sly) < tolerance_sq && abs(slx - slz) < tolerance_sq;
	}

	template <class T>
	Mat4<T> Mat4<T>::operator*(const Mat4& o) const
	{
		return Mat4<T>((*this) * o.x_, (*this) * o.y_, (*this) * o.z_, (*this) * o.w_);
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
	CP_FORCE_INLINE Mat4<T> translate(const Mat4<T>& m, const Vec3<T>& v)
	{
		Mat4<T> result = m;
		result.w_ = m * Vec4<T>(v, 1.0f);
		return result;
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
		T dot1 = (dot0.x_ + dot0.y_) + (dot0.z_ + dot0.w_);

		T one_over_determinant = static_cast<T>(1) / dot1;

		return inv * one_over_determinant;
	}

	template <class T>
	Mat4<T> mat4_cast(const Quat<T>& q)
	{
		const Mat3<T> mat3 = mat3_cast(q);
		return Mat4<T>(Vec4<T>(mat3.x_, 0.0f), Vec4<T>(mat3.y_, 0.0f), Vec4<T>(mat3.z_, 0.0f), Vec4<T>::unit_w());
	}

	template <class T>
	Mat4<T> mat4_cast(const Transform<T>& t)
	{
		Mat4<T> m = mat4_cast(t.get_rotation());
		const T scale = t.get_scale();
		m.x_ *= scale;
		m.y_ *= scale;
		m.z_ *= scale;
		m.w_ = Vec4<T>(t.get_translation(), static_cast<T>(1));
		return m;
	}

	template <class T>
	Quat<T> quat_cast(const Mat4<T>& m)
	{
		const Mat3<T> mat3(m.x_.xyz(), m.y_.xyz(), m.z_.xyz());
		return quat_cast(mat3);
	}

	template <class T>
	Transform<T> transform_cast(const Mat4<T>& m)
	{
		CP_ASSERT(m.is_scale_uniform());
		return Transform<T>(m.w_.xyz(), normalize(quat_cast(m)), m.x_.length());
	}

	// Unlike GLM, produces a transform matrix, without the inverse of a view matrix used for rendering
	// So this is well suited for camera transforms or any gameplay transforms. The inverse happens during rendering when inverting the camera transform.
	template <class T>
	Mat4<T> look_at(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up)
	{
		const Vec3<T> f(normalize(center - eye));
		const Vec3<T> l(normalize(cross(up, f)));
		const Vec3<T> u(cross(f, l));
		return Mat4<T>(Vec4<T>(l, 0), Vec4<T>(u, 0), Vec4<T>(f, 0), Vec4<T>(eye, 1) );
	}

	template <class T>
	Mat4<T> perspective(T fovy, T aspect, T z_near, T z_far)
	{
		// From glm::perspectiveRH_ZO

		CP_ASSERT(aspect > static_cast<T>(0));
		T const tan_half_fovy = tan(fovy / static_cast<T>(2));
		Mat4<T> result(static_cast<T>(0));
		result[0][0] = static_cast<T>(1) / (aspect * tan_half_fovy);
		result[1][1] = static_cast<T>(1) / (tan_half_fovy);
		result[2][2] = z_far / (z_near - z_far);
		result[2][3] = -static_cast<T>(1);
		result[3][2] = -(z_far * z_near) / (z_far - z_near);
		return result;
	}
}
