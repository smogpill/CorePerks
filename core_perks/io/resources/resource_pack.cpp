// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_pack.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	void ResourcePack::add_resource(const UntypedResourceHandle& handle)
	{
		if (!contains(resources_, handle))
			resources_.push_back(handle);
	}
}
