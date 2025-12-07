// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_handle.h"

namespace cp
{
	class ResourcePack : public Resource
	{
	public:
		void add_resource(const UntypedResourceHandle& handle);

	private:
		std::vector<UntypedResourceHandle> _resources;
	};
}
