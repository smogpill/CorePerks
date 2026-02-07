// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#ifdef CP_JOLT
namespace cp::jolt
{
	CP_FORCE_INLINE JPH::Vec3 jph_vec3_cast(const Vec3f& v)
	{
		return JPH::Vec3(v.x_, v.y_, v.z_);
	}

	CP_FORCE_INLINE const Vec3f& vec3_cast(const JPH::Vec3& v)
	{
		return reinterpret_cast<const Vec3f&>(v);
	}

	CP_FORCE_INLINE const JPH::Quat& jph_quat_cast(const Quatf& q)
	{
		return reinterpret_cast<const JPH::Quat&>(q);
	}

	CP_FORCE_INLINE const Quatf& quat_cast(const JPH::Quat& q)
	{
		return reinterpret_cast<const Quatf&>(q);
	}

#ifdef JPH_DOUBLE_PRECISION
	CP_FORCE_INLINE JPH::RVec3 jph_rvec3_cast(const Vec3d& v)
	{
		return JPH::RVec3(v.x_, v.y_, v.z_);
	}

	CP_FORCE_INLINE const Vec3d& vec3_cast(const JPH::RVec3& v)
	{
		return reinterpret_cast<const Vec3d&>(v);
	}
#endif
}
#endif
