// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_pack.h"

namespace cp
{
	void ResourcePack::add_resource(const UntypedResourceHandle& handle)
	{
		if (!contains(_resources, handle))
			_resources.push_back(handle);
	}
}
