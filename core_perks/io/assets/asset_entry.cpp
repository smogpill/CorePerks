// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/base/asset_base.h"
#include "core_perks/io/assets/asset_entry.h"
#include "core_perks/io/assets/asset_manager.h"
#include "core_perks/io/assets/asset_loader.h"
#include "core_perks/io/assets/asset_generator.h"
#include "core_perks/io/assets/asset.h"
#include "core_perks/io/streams.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    AssetEntry::AssetEntry(const std::string& id, uint64 id_hash, const Type& type)
        : id_(id)
        , id_hash_(id_hash)
        , type_(&type)
    {
    }

    AssetEntry::~AssetEntry()
    {
        delete resource_;
        delete loading_resource_;
    }

    void AssetEntry::set_generator(AssetGenerator* generator)
    {
		std::scoped_lock lock(mutex_);
		generator_ = generator;
    }

    void AssetEntry::add_loading_dependency()
    {
        ++nb_loading_dependencies_;
    }

    void AssetEntry::remove_loading_dependency()
    {
        if (--nb_loading_dependencies_ != 0)
            return;
        callback_mutex_.lock();

        Asset* loaded_resource = loading_resource_.exchange(nullptr);

        // Notify that all dependencies have been loaded
        if (loading_result_)
            loading_result_ = loaded_resource->on_dependencies_loaded();

        // Swap the resource and update the state
        if (loading_result_)
        {
            Asset* old_resource = resource_.exchange(loaded_resource);
            delete old_resource;
            state_ = AssetState::READY;
        }
        else
        {
            delete loaded_resource;
            if (state_ != AssetState::READY)
                state_ = AssetState::FAILED;
        }

        // Call the callbacks
        std::vector<std::function<void(bool)>> callbacks = std::move(load_callbacks_);
        callback_mutex_.unlock();
        for (auto& cb : callbacks)
        {
            cp::JobSystem::get().enqeue([callback = std::move(cb), loading_result = loading_result_]() { callback(loading_result); });
        }

        // Notify the parent resource
        if (loading_parent_)
        {
            if (!loading_result_)
                loading_parent_->loading_result_ = false;
            loading_parent_->remove_loading_dependency();
            loading_parent_ = nullptr;
        }
    }

    void AssetEntry::on_all_refs_removed()
    {
        Base::on_all_refs_removed();
    }

    void AssetEntry::add_load_callback(Callback callback)
    {
        std::scoped_lock lock(callback_mutex_);
        switch (state_)
        {
        case AssetState::READY:
        case AssetState::FAILED:
        {
            queue_async_callback(std::move(callback));
            break;
        }
        default:
        {
            load_callbacks_.push_back(std::move(callback));
            break;
        }
        }
    }

    void AssetEntry::on_update_async()
    {
        std::scoped_lock lock(mutex_);
        for (;;)
        {
            const AssetState target_state = user_refs_ ? AssetState::READY : AssetState::NONE;
            if (state_ == target_state)
                break;

            if (target_state == AssetState::READY)
            {
                switch (state_)
                {
                    switch (state_)
                    {
                    case AssetState::NONE:
                    {

                        break;
                    }
                    case AssetState::READY:
                    {
                        break;
                    }
                    case AssetState::FAILED:
                    {
                        break;
                    }
                    case AssetState::LOADING_DEPENDENCIES:
                    {
                        break;
                    }
                    case AssetState::LOADING_SELF:
                    {
                        break;
                    }
                    case AssetState::PROCESSING:
                    {
                        break;
                    }
                    case AssetState::STORING:
                    {
                        break;
                    }
                    case AssetState::UNLOADING:
                    {
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }
                }
            }
            else
            {

            }
        }
    }

    void AssetEntry::on_load_dependencies()
    {
        std::scoped_lock lock(mutex_);
        if (dependencies_.empty())
        {
			state_ = AssetState::LOADING_SELF;
            return;
        }
        state_ = AssetState::LOADING_DEPENDENCIES;
        nb_loading_dependencies_ = dependencies_.size();
        for (AssetHandle& dep : dependencies_)
            dep.load_async(&on_dependency_loaded);
    }

    void AssetEntry::on_dependency_loaded(AssetEntry& entry)
    {
        if (nb_loading_dependencies_.fetch_sub(1) > 1)
            return;

        std::scoped_lock lock(mutex_);
        for (const AssetHandle& dep : dependencies_)
            if (!dep->is_ready())
            {
                state_ = AssetState::FAILED;
                return;
            }

        state_ = AssetState::LOADING_SELF;
    }

    void AssetEntry::flush_load_callbacks()
    {
		std::vector<Callback> callbacks = std::move(load_callbacks_);
        for (auto& callback : callbacks)
            queue_async_callback(std::move(callback));
    }

    void AssetEntry::queue_async_callback(Callback&& callback)
    {
        add_ref();
        auto job = [this, callback = std::move(callback)]()
        {
            callback(*this);
            remove_ref();
        };
		cp::JobSystem::get().enqeue(job);
    }

    Asset* AssetEntry::create_resource()
    {
        CP_ASSERT(type_);
        Asset* resource = type_->create<Asset>();
        resource->entry_ = this;
        return resource;
    }

    void AssetEntry::on_load()
    {
        std::scoped_lock lock(mutex_);
		std::unique_ptr<Asset> resource(create_resource());
        if (!resource->on_load(AssetLoader(*this)))
        {
            state_ = AssetState::FAILED;
            return;
        }

        Asset* old_resource = resource_.exchange(resource.release());
        delete old_resource;
        state_ = AssetState::READY;
    }

    void AssetEntry::on_unload()
    {
        std::scoped_lock lock(mutex_);
        for (AssetHandle& dep : dependencies_)
            dep.unload_async();
    }

    void AssetEntry::load_async(Callback on_done)
    {
        AssetState expected = AssetState::NONE;
        if (!target_state_.compare_exchange_strong(expected, AssetState::READY))
        {

        }
        add_load_callback(on_done);
        
        /*
        if (state_.compare_exchange_strong(expected, AssetState::LOADING))
        {
            add_ref();
            cp::JobSystem::get().enqeue([this]()
                {
                    Resource* new_resource = type_->create<Resource>();
                    Resource* old_resource = loading_resource_.exchange(new_resource);
                    delete old_resource;
                    ResourceLoader loader(*this);
                    add_loading_dependency();
                    loading_result_ = loading_resource_.load()->on_load(loader);
                    remove_loading_dependency();
                    remove_ref();
                });
        }
        */
    }

    void AssetEntry::unload_async()
    {
		std::mutex lock(mutex_);
        if (state_ == AssetState::READY)
        {

        }
        AssetState expected = AssetState::READY;
        if (state_.compare_exchange_strong(expected, AssetState::RELEASING))
        {
            
        }
    }

    void AssetEntry::store_async(Callback on_done)
    {
        add_ref();

        cp::JobSystem::get().enqeue([this, callback = std::move(on_done)]()
            {
                AssetState expected = AssetState::READY;
                while (!state_.compare_exchange_weak(expected, AssetState::SERIALIZING))
                {
                    if (expected != AssetState::SERIALIZING)
                    {
                        callback(false);
                        return;
                    }
                    expected = AssetState::READY;
                }
                const Asset* resource = resource_;
                cp::BinaryOutputStream stream;
                resource->on_store(stream);

                expected = AssetState::SERIALIZING;
                const bool was_serializing = state_.compare_exchange_strong(expected, AssetState::READY);
                CP_ASSERT(was_serializing);

                namespace fs = std::filesystem;

                const fs::path file_path = get_asset_path();

                remove_ref();

                {
                    auto parent_path = file_path.parent_path();
                    if (!parent_path.empty())
                        fs::create_directories(parent_path);
                }

                std::ofstream file(file_path, std::ios::binary);
                if (!file.is_open())
                {
                    CP_ERROR("Failed to open for write: {}", file_path.string());
                    callback(false);
                    return;
                }

                const std::string content = stream.str();

                file.write(content.data(), content.size());

                if (!file.good())
                {
                    CP_ERROR("Failed to write: {}", file_path.string());
                    callback(false);
                    return;
                }

                file.close();

                callback(true);
            });
    }

    bool AssetEntry::path_exists() const
    {
        return std::filesystem::exists(get_asset_path());
    }

    std::string AssetEntry::get_name() const
    {
        const size_t pos = id_.find_last_of('/');
        return (pos == std::string::npos) ? id_ : id_.substr(pos + 1);
    }

    void AssetEntry::set(Asset* resource)
    {
        Asset* old_resource = resource_.exchange(resource);
        delete old_resource;
    }

    std::string AssetEntry::get_asset_path() const
    {
        return (std::filesystem::path(AssetManager::get().get_assets_path()) / get_id()).string();
    }

    void AssetEntry::update_async()
    {
        std::scoped_lock lock(mutex_);
		const AssetState target_state = user_refs_ ? AssetState::READY : AssetState::NONE;
        if (target_state != state_)
        {
            add_ref();
            cp::JobSystem::get().enqeue([this]()
                {
                    on_update_async();
                    remove_ref();
                });
        }
    }
}
