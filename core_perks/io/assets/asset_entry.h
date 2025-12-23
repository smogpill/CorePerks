// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/base/asset_base.h"
#include "core_perks/io/assets/asset_handle.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class AssetLoader; class Asset; class AssetGenerator;

	template <class T>
	struct LoadResult
	{
		std::unique_ptr<T> resource_ = nullptr;
		bool result_ = false;
	};

	class AssetEntry : public RefCounted
	{
		using Base = RefCounted;
	public:
		using Callback = std::function<void(AssetEntry&)>;
		AssetEntry(const std::string& id, uint64 id_hash, const Type& type);
		virtual ~AssetEntry();

		const std::string& get_id() const { return id_; }
		std::string get_name() const;
		uint64 get_id_hash() const { return id_hash_; }
		void set_generator(AssetGenerator* generator);
		void add_loading_dependency();
		void remove_loading_dependency();
		//void add_load_request(AssetHandle& handle);
		//void remove_load_request(AssetHandle& handle);
		void load_async(Callback callback);
		bool path_exists() const;
		void unload_async();
		void store_async(Callback callback);
		Asset* get() const { return resource_; }
		void set(Asset* asset);
		std::string get_asset_path() const;
		bool is_ready() const { return state_ == AssetState::READY; }
		void update_async();
		MappedAssetData get_mapped_data();

	private:
		friend class AssetLoader;
		friend class AssetManager;
		void on_all_refs_removed() override;
		void add_load_callback(Callback callback);
		void on_update_async();
		void on_load_dependencies();
		void on_load();
		void on_unload();
		void on_dependency_loaded(AssetEntry& entry);
		void flush_load_callbacks();
		void queue_async_callback(Callback&& callback);
		Asset* create_resource();

		std::string id_;
		uint64 id_hash_ = 0;
		AssetPriority priority_;
		const cp::Type* type_ = nullptr;
		RefPtr<AssetEntry> loading_parent_;
		std::atomic<uint32> nb_loading_dependencies_ = 0;
		std::mutex callback_mutex_;
		std::atomic<AssetState> state_ = AssetState::NONE;
		std::vector<AssetHandle*> load_requests_;
		std::atomic<uint32> load_refs_ = 0;
		std::vector<Callback> load_callbacks_;
		std::atomic<Asset*> resource_ = nullptr;
		std::atomic<Asset*> loading_resource_ = nullptr;
		bool loading_result_ = false;
		bool locked_ = false;
		std::mutex mutex_;
	};
}
