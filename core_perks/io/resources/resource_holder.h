// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class ResourceLoader;
	class Resource;

	enum class ResourceState
	{
		// Stable states
		NONE,
		READY,
		FAILED,

		// Transitions
		LOADING_DEPENDENCIES,
		LOADING_SELF,
		PROCESSING,
		SERIALIZING,
		RELEASING
	};

	template <class T>
	struct LoadResult
	{
		std::unique_ptr<T> _resource = nullptr;
		bool _result = false;
	};

	class ResourceHolder : public RefCounted<ResourceHolder>
	{
		using Base = RefCounted<ResourceHolder>;
	public:
		using Callback = std::function<void(bool)>;
		using CreateFunc = std::function<Resource*()>;
		ResourceHolder(const std::string& id);
		virtual ~ResourceHolder();

		void add_load_callback(Callback callback);
		auto get_id() const -> const std::string& { return _id; }
		auto get_name() const -> std::string;
		auto get_id_hash() const -> uint64 { return _id_hash; }
		void add_loading_dependency();
		void remove_loading_dependency();
		void load_async(std::function<Resource* ()> create_func);
		bool path_exists() const;
		void unload_async();
		void store_async(Callback callback);
		auto get() const -> Resource* { return _resource; }
		void set(Resource* resource);
		auto get_asset_path() const -> std::string;

	protected:
		friend class ResourceLoader;
		friend class ResourceManager;
		void on_all_refs_removed() override;

		std::string _id;
		uint64 _id_hash = 0;
		RefPtr<ResourceHolder> _loading_parent;
		std::atomic<uint32> _nb_loading_dependencies = 0;
		std::mutex _callback_mutex;
		std::atomic<ResourceState> _state = ResourceState::NONE;
		std::vector<Callback> _load_callbacks;
		std::atomic<Resource*> _resource = nullptr;
		std::atomic<Resource*> _loading_resource = nullptr;
		/// TODO: Replace by RTTI
		CreateFunc _create_func;
		bool _loading_result = false;
	};
}
