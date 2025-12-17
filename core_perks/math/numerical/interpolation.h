// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/quat.h"

namespace cp
{
	template <class T> CP_FORCE_INLINE T lerp(T a, T b, T t) { return fma(b - a, t, a); }

	template <class T>
	CP_FORCE_INLINE Quat<T> slerp(const Quat<T>& a, const Quat<T>& b, T t)
	{
		Quat<T> b_mod = b;
		const T dot_ab = dot(a, b);
		if (dot_ab < T(0))
		{
			b_mod = -b;
		}
		const T dot_a_bmod = dot(a, b_mod);
		if (dot_a_bmod > T(0.9995))
		{
			return normalize(a * (T(1) - t) + b_mod * t);
		}
		else
		{
			const T theta_0 = acos(dot_a_bmod);
			const T theta = theta_0 * t;
			const T sin_theta = sin(theta);
			const T sin_theta_0 = sin(theta_0);
			const T s0 = cos(theta) - dot_a_bmod * sin_theta / sin_theta_0;
			const T s1 = sin_theta / sin_theta_0;
			return a * s0 + b_mod * s1;
		}
	}

	template <typename T>
	CP_FORCE_INLINE T smooth(T& velocity, const T& from, const T& to, const T& smooth_time, const float dt)
	{
		const T Omega4 = T(8) / (smooth_time + T(0.0001)); // small value to avoid division by 0
		const T X = Omega4 * dt;
		const T X2 = X * X;
		const T Denom = (T(1) + X + T(0.48) * X2 + T(0.235) * X2 * X);
		const T Exp = T(1) / Denom;
		const T Change = from - to;
		const T Temp = (velocity + (Change * Omega4)) * T(dt);
		velocity = (velocity - (Temp * Omega4)) * Exp;
		return to + (Change + Temp) * Exp;
	}

	template <typename T>
	CP_FORCE_INLINE T cubic_hermite(T a, T b, T c, T d, float t)
	{
		const T aa = -a / 2 + (3 * b) / 2 - (3 * c) / 2 + d / 2;
		const T bb = a - (5 * b) / 2 + 2 * c - d / 2;
		const T cc = -a / 2 + c / 2;
		const T dd = b;
		return aa * t * t * t + bb * t * t + cc * t + dd;
	}
}
