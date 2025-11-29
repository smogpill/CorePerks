// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_holder.h"

namespace cp
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager(const std::string& assets_path);
		~ResourceManager();

		auto get_assets_path() const -> const std::string& { return _assets_path; }

	private:
		friend class ResourceHolder;
		friend class ResourceHandleBase;

		auto get_or_create_holder(const std::string& id) -> ResourceHolder*;
		void destroy_holder(ResourceHolder& holder);

		mutable std::mutex _mutex;
		std::unordered_map<std::string, ResourceHolder*> _map;
		std::queue<ResourceHandleBase*> _load_queue;
		std::string _assets_path;
	};
}
