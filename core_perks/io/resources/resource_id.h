// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/patterns/hashed_string.h"

namespace cp
{
	class ResourceID : public HashedString
	{
	public:
		using HashedString::HashedString;
		using HashedString::operator=;

		/// Returns the file name without the extension
		std::string get_name() const;
		static const ResourceID& get_empty();
	};
}
