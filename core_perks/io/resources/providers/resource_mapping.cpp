// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/providers/resource_mapping.h"

#if 0

namespace cp
{
	MappedResourceData::MappedResourceData(const ResourceHandle& resource)
		: resource_(resource)
	{
	}

	MappedResourceData::MappedResourceData(const ResourceHandle& resource, const ResourceHandle& provider, MappedFileRegion&& region)
		: resource_(resource), provider_(provider), region_(std::move(region))
	{
	}
}

#endif