// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/hash.h"

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

	using hash_resource_id = hash::strong::hash64;
}
