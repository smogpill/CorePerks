// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/streams/binary_serializer.h"
#include "core_perks/containers/vector_extensions.h"
#include "resource_folder.h"

namespace cp
{
	CP_DEFINE_CLASS(Resource)
	{
	}
	
	void Resource::add_dependency(const ResourceHandle& handle)
	{
		CP_ASSERT(handle);
		if (!contains(dependencies_, handle))
			dependencies_.push_back(handle);
	}

	void Resource::on_serialize(BinarySerializer& serializer)
	{
		serializer / dependencies_;
	}

	bool Resource::on_load()
	{
		return true;
	}

	void Resource::on_unload()
	{
		dependencies_.clear();
		dependents_.clear();
	}

	const ResourceID& Resource::get_id() const
	{
		return entry_ ? entry_->get_id() : ResourceID::get_empty();
	}

	ResourceHandle Resource::get_handle() const
	{
		return ResourceHandle(entry_);
	}
}
