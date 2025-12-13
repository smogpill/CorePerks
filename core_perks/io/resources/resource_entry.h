// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class ResourceLoader;
	class Resource;

	template <class T>
	struct LoadResult
	{
		std::unique_ptr<T> resource_ = nullptr;
		bool result_ = false;
	};

	class ResourceEntry : public RefCounted
	{
		using Base = RefCounted;
	public:
		using Callback = std::function<void(bool)>;
		ResourceEntry(const std::string& id, uint64 id_hash, const Type& type);
		virtual ~ResourceEntry();

		const std::string& get_id() const { return id_; }
		std::string get_name() const;
		uint64 get_id_hash() const { return id_hash_; }
		void add_loading_dependency();
		void remove_loading_dependency();
		void load_async(Callback callback);
		bool path_exists() const;
		void unload_async();
		void store_async(Callback callback);
		Resource* get() const { return resource_; }
		void set(Resource* resource);
		std::string get_asset_path() const;

	protected:
		friend class ResourceLoader;
		friend class ResourceManager;
		void on_all_refs_removed() override;
		void add_load_callback(Callback callback);

		std::string id_;
		uint64 id_hash_ = 0;
		const cp::Type* type_ = nullptr;
		RefPtr<ResourceEntry> loading_parent_;
		std::atomic<uint32> nb_loading_dependencies_ = 0;
		std::mutex callback_mutex_;
		std::atomic<ResourceState> state_ = ResourceState::NONE;
		std::atomic<ResourceState> target_state_ = ResourceState::NONE;
		std::vector<Callback> load_callbacks_;
		std::atomic<Resource*> resource_ = nullptr;
		std::atomic<Resource*> loading_resource_ = nullptr;
		std::vector<UntypedResourceHandle> dependencies_;

		bool loading_result_ = false;
		std::mutex mutex_;
	};
}
