// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/quat.h"
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	template <class T>
	class alignas(Vec3<T>) Transform
	{
	public:
		CP_FORCE_INLINE Transform() : translation_scale_(Vec4<T>::unit_w()) {}
		CP_FORCE_INLINE Transform(const Vec3<T>& translation, const Quat<T>& rotation, T scale)
			: translation_scale_(translation.x_, translation.y_, translation.z_, scale), rotation_(rotation) {}
		CP_FORCE_INLINE const Vec3<T>& get_translation() const { return translation_scale_.xyz(); }
		CP_FORCE_INLINE void set_translation(const Vec3<T>& t) { translation_scale_.x_ = t.x_; translation_scale_.y_ = t.y_; translation_scale_.z_ = t.z_; }
		CP_FORCE_INLINE void set_scale(T s) { translation_scale_.w_ = s; }
		CP_FORCE_INLINE T get_scale() const { return translation_scale_.w_; }
		CP_FORCE_INLINE void set_rotation(const Quat<T>& rotation) { rotation_ = rotation; }
		CP_FORCE_INLINE const Quat<T>& get_rotation() const { return rotation_; }
		CP_FORCE_INLINE Vec3<T> get_left() const { return rotation_ * Vec3<T>::unit_x(); }
		CP_FORCE_INLINE Vec3<T> get_up() const { return rotation_ * Vec3<T>::unit_y(); }
		CP_FORCE_INLINE Vec3<T> get_forward() const { return rotation_ * Vec3<T>::unit_z(); }
		Vec3<T> transform_position(const Vec3<T>& x) const;
		Vec3<T> transform_vector(const Vec3<T>& x) const;
		Vec3<T> inverse_transform_position(const Vec3<T>& x) const;
		Vec3<T> inverse_transform_vector(const Vec3<T>& x) const;

		Transform<T> operator*(const Transform<T>& o) const;
		Transform<T>& operator*=(const Transform<T>& o) { *this = *this * o; return *this; }

	private:
		Vec4<T> translation_scale_;
		Quat<T> rotation_;
	};

	static_assert(sizeof(Transform<float>) == 32);
	static_assert(sizeof(Transform<double>) == 64);

	using Transformf = Transform<float>;
	using Transformd = Transform<double>;

	template <class T>
	CP_FORCE_INLINE Vec3<T> Transform<T>::transform_position(const Vec3<T>& _pos) const
	{
		return rotation_ * (get_scale() * _pos) + get_translation();
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> Transform<T>::transform_vector(const Vec3<T>& _vec) const
	{
		return rotation_ * (get_scale() * _vec);
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> Transform<T>::inverse_transform_position(const Vec3<T>& _pos) const
	{
		return inverse(rotation_) * (_pos - get_translation()) / get_scale();
	}

	template <class T>
	CP_FORCE_INLINE Vec3<T> Transform<T>::inverse_transform_vector(const Vec3<T>& _vec) const
	{
		return inverse(rotation_) * _vec / get_scale();
	}

	template <class T>
	CP_FORCE_INLINE Transform<T> Transform<T>::operator* (const Transform<T>& b) const
	{
		return Transform<T>(
			rotation_ * (b.get_translation() * get_scale()) + get_translation(),
			rotation_ * b.rotation_,
			get_scale() * b.get_scale()
		);
	}

	template <class T>
	CP_FORCE_INLINE Transform<T> inverse(const Transform<T>& t)
	{
		const T inv_scale = T(1) / t.get_scale();
		const Quat<T> inv_rotation = conjugate(t.get_rotation());
		const Vec3<T> inv_translation = inv_rotation * (-t.get_translation() * inv_scale);
		return Transform<T>(inv_translation, inv_rotation, inv_scale);
	}
}
