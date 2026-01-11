// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_id.h"

namespace cp
{
	std::string ResourceID::get_name() const
	{
		std::filesystem::path path = string();
		return path.stem().string();
	}

	const ResourceID& ResourceID::get_empty()
	{
		static const ResourceID empty;
		return empty;
	}
}
