// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/numerical/hash.h"

namespace cp
{
	enum class ResourceState
	{
		// Stable states
		NONE,
		READY,
		FAILED,

		// Transitions
		LOADING_DEPENDENCIES,
		LOADING_SELF,
		PROCESSING,
		SERIALIZING,
		RELEASING
	};

	namespace hash
	{
		namespace resource_id
		{
			using strong::hash64;
			using strong::hash64_compile_time;
		}
	}
}
