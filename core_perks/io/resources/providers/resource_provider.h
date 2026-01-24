// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/resource_mapping.h"

namespace cp
{
	class ResourceID; class ResourceHandle;

	class ResourceProvider
	{
	public:
		virtual ~ResourceProvider() = default;

		virtual bool has_resource(const ResourceID& id) const = 0;
		virtual ResourceMapping map_resource(const ResourceID& id) = 0 { return ResourceMapping(); }
		virtual void store_resource_async(const ResourceID& id, const RefPtr<Resource>& resource, std::function<void(bool)>&& on_done = [](bool) {}) { on_done(false); }

	protected:
	};
}
