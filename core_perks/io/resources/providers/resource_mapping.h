// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/io/resources/resource_handle.h"

namespace cp
{
	struct ResourceMapping
	{
		enum class Status
		{
			MISSING,
			FAILED,
			SUCCESS
		};

		Status status_ = Status::MISSING;
		MappedFileRegion region_;
		ResourceHandle provider_;
	};
}
