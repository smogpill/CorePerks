// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_manager.h"
#include "resource_folder.h"

namespace cp
{
	CP_DEFINE_CLASS(Resource)
	{
	}
	
	void Resource::add_dependency(const ResourceHandle& handle)
	{
		if (!contains(dependencies_, handle))
			dependencies_.push_back(handle);
	}
}
