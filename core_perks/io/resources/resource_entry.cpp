// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/streams.h"
#include "core_perks/math/hash.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    ResourceEntry::ResourceEntry(const std::string& id, uint64 id_hash, const Type& type)
        : id_(id)
        , id_hash_(id_hash)
        , type_(&type)
    {
    }

    ResourceEntry::~ResourceEntry()
    {
        delete resource_;
        delete loading_resource_;
    }

    void ResourceEntry::add_loading_dependency()
    {
        ++nb_loading_dependencies_;
    }

    void ResourceEntry::remove_loading_dependency()
    {
        if (--nb_loading_dependencies_ == 0)
        {
            callback_mutex_.lock();

            Resource* loaded_resource = loading_resource_.exchange(nullptr);

            // Notify that all dependencies have been loaded
            if (loading_result_)
                loading_result_ = loaded_resource->on_dependencies_loaded();

            // Swap the resource and update the state
            if (loading_result_)
            {
                Resource* old_resource = resource_.exchange(loaded_resource);
                delete old_resource;
                state_ = ResourceState::READY;
            }
            else
            {
                delete loaded_resource;
                if (state_ != ResourceState::READY)
                    state_ = ResourceState::FAILED;
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
    }

    void ResourceEntry::on_all_refs_removed()
    {
        ResourceManager::get().destroy_entry(*this);
        Base::on_all_refs_removed();
    }

    void ResourceEntry::add_load_callback(Callback callback)
    {
        std::scoped_lock lock(callback_mutex_);
        switch (state_)
        {
        case ResourceState::READY:
        {
            cp::JobSystem::get().enqeue([cb = std::move(callback)]() { cb(true); });
            break;
        }
        case ResourceState::FAILED:
        {
            cp::JobSystem::get().enqeue([cb = std::move(callback)]() { cb(false); });
            break;
        }
        default:
        {
            load_callbacks_.push_back(std::move(callback));
            break;
        }
        }
    }

    void ResourceEntry::load_async(Callback on_done)
    {
        ResourceState expected = ResourceState::NONE;
        if (!target_state_.compare_exchange_strong(expected, ResourceState::READY))
        {

        }
        add_load_callback(on_done);
        
        if (state_.compare_exchange_strong(expected, ResourceState::LOADING))
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
    }

    void ResourceEntry::unload_async()
    {
        ResourceState expected = ResourceState::READY;
        if (state_.compare_exchange_strong(expected, ResourceState::RELEASING))
        {
            add_ref();
            cp::JobSystem::get().enqeue([this]()
                {
                    Resource* resource = resource_.exchange(nullptr);
                    delete resource;
                    remove_ref();
                });
        }
    }

    void ResourceEntry::store_async(Callback on_done)
    {
        add_ref();

        cp::JobSystem::get().enqeue([this, callback = std::move(on_done)]()
            {
                ResourceState expected = ResourceState::READY;
                while (!state_.compare_exchange_weak(expected, ResourceState::SERIALIZING))
                {
                    if (expected != ResourceState::SERIALIZING)
                    {
                        callback(false);
                        return;
                    }
                    expected = ResourceState::READY;
                }
                const Resource* resource = resource_;
                cp::BinaryOutputStream stream;
                resource->on_store(stream);

                expected = ResourceState::SERIALIZING;
                const bool was_serializing = state_.compare_exchange_strong(expected, ResourceState::READY);
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

    bool ResourceEntry::path_exists() const
    {
        return std::filesystem::exists(get_asset_path());
    }

    auto ResourceEntry::get_name() const -> std::string
    {
        const size_t pos = id_.find_last_of('/');
        return (pos == std::string::npos) ? id_ : id_.substr(pos + 1);
    }

    void ResourceEntry::set(Resource* resource)
    {
        Resource* old_resource = resource_.exchange(resource);
        delete old_resource;
    }

    auto ResourceEntry::get_asset_path() const -> std::string
    {
        return (std::filesystem::path(ResourceManager::get().get_assets_path()) / get_id()).string();
    }
}
