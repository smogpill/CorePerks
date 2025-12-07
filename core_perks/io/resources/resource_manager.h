// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_handle.h"

namespace cp
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();

		// Assets path
		void set_assets_path(const std::string& path);
		auto get_assets_path() const -> const std::string& { return _assets_path; }

		// Cache path
		void set_cache_path(const std::string& path);
		auto get_cache_path() const -> const std::string& { return _cache_path; }

	private:
		friend class ResourceEntry;
		friend class UntypedResourceHandle;

		auto get_or_create_entry(const std::string& id) -> ResourceEntry*;
		void destroy_entry(ResourceEntry& entry);
		void add_request(const UntypedResourceHandle& request);

		mutable std::mutex _mutex;
		std::unordered_map<uint64, ResourceEntry*> _map;
		std::queue<UntypedResourceHandle> _requests;
		std::string _assets_path;
		std::string _cache_path;

		/*
		Streaming
		std::queue<Request> _immediate_queue; // Must load now (player weapon, current mission)
		std::queue<Request> _high_priority_queue; // Visible/nearby
		std::queue<Request> _low_priority_queue; // Predictive/prefetch
		std::queue<Request> _unload_queue;
		*/
	};
}
