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
		const std::string& get_assets_path() const { return assets_path_; }

		// Cache path
		void set_cache_path(const std::string& path);
		const std::string& get_cache_path() const { return cache_path_; }

	private:
		friend class ResourceEntry;
		friend class UntypedResourceHandle;

		ResourceEntry* get_or_create_entry(const std::string& id, const Type& type);
		void destroy_entry(ResourceEntry& entry);
		void add_request(const UntypedResourceHandle& request);

		mutable std::mutex mutex_;
		std::unordered_map<uint64, ResourceEntry*> map_;
		std::queue<UntypedResourceHandle> requests_;
		std::string assets_path_;
		std::string cache_path_;

		/*
		Streaming
		std::queue<Request> immediate_queue_; // Must load now (player weapon, current mission)
		std::queue<Request> high_priority_queue_; // Visible/nearby
		std::queue<Request> low_priority_queue_; // Predictive/prefetch
		std::queue<Request> unload_queue_;
		*/
	};
}
