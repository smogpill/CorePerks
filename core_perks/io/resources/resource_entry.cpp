// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource_generator.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/streams.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/threading/job/job_system.h"

#if 0

namespace cp
{
    ResourceEntry::ResourceEntry(const HashedString& id, const Type& type)
        : id_(id)
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
        if (--nb_loading_dependencies_ != 0)
            return;
    }

    void ResourceEntry::on_all_refs_removed()
    {
        Base::on_all_refs_removed();
    }

    void ResourceEntry::add_load_callback(Callback callback)
    {
        std::scoped_lock lock(callback_mutex_);
        switch (state_)
        {
        case ResourceState::READY:
        case ResourceState::FAILED:
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

    void ResourceEntry::on_update_async()
    {
        std::scoped_lock lock(mutex_);
        for (;;)
        {
            const ResourceState target_state = user_refs_ ? ResourceState::READY : ResourceState::NONE;
            if (state_ == target_state)
                break;

            if (target_state == ResourceState::READY)
            {
                switch (state_)
                {
                    switch (state_)
                    {
                    case ResourceState::NONE:
                    {

                        break;
                    }
                    case ResourceState::READY:
                    {
                        break;
                    }
                    case ResourceState::FAILED:
                    {
                        break;
                    }
                    case ResourceState::LOADING_DEPENDENCIES:
                    {
                        break;
                    }
                    case ResourceState::LOADING_SELF:
                    {
                        break;
                    }
                    case ResourceState::PROCESSING:
                    {
                        break;
                    }
                    case ResourceState::STORING:
                    {
                        break;
                    }
                    case ResourceState::UNLOADING:
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

    void ResourceEntry::on_load_dependencies()
    {
        std::scoped_lock lock(mutex_);
        if (dependencies_.empty())
        {
			state_ = ResourceState::LOADING_SELF;
            return;
        }
        state_ = ResourceState::LOADING_DEPENDENCIES;
        nb_loading_dependencies_ = dependencies_.size();
        for (ResourceHandle& dep : dependencies_)
            dep.load_async(&on_dependency_loaded);
    }

    void ResourceEntry::on_dependency_loaded(ResourceEntry& entry)
    {
        if (nb_loading_dependencies_.fetch_sub(1) > 1)
            return;

        std::scoped_lock lock(mutex_);
        for (const ResourceHandle& dep : dependencies_)
            if (!dep->is_ready())
            {
                state_ = ResourceState::FAILED;
                return;
            }

        state_ = ResourceState::LOADING_SELF;
    }

    void ResourceEntry::flush_load_callbacks()
    {
		std::vector<Callback> callbacks = std::move(load_callbacks_);
        for (auto& callback : callbacks)
            queue_async_callback(std::move(callback));
    }

    void ResourceEntry::queue_async_callback(Callback&& callback)
    {
        add_ref();
        auto job = [this, callback = std::move(callback)]()
        {
            callback(*this);
            remove_ref();
        };
		JobSystem::get().enqeue(job);
    }

    Resource* ResourceEntry::create_resource()
    {
        CP_ASSERT(type_);
        Resource* resource = type_->create<Resource>();
        resource->entry_ = this;
        return resource;
    }

    void ResourceEntry::on_load()
    {
        std::scoped_lock lock(mutex_);
		std::unique_ptr<Resource> resource(create_resource());
        if (!resource->on_load(ResourceLoader(*this)))
        {
            state_ = ResourceState::FAILED;
            return;
        }

        Resource* old_resource = resource_.exchange(resource.release());
        delete old_resource;
        state_ = ResourceState::READY;
    }

    void ResourceEntry::on_unload()
    {
        std::scoped_lock lock(mutex_);
        for (ResourceHandle& dep : dependencies_)
            dep.unload_async();
    }

    void ResourceEntry::load_async(Callback on_done)
    {
        ResourceState expected = ResourceState::NONE;
        if (!target_state_.compare_exchange_strong(expected, ResourceState::READY))
        {

        }
        add_load_callback(on_done);
        
        /*
        if (state_.compare_exchange_strong(expected, ResourceState::LOADING))
        {
            add_ref();
            JobSystem::get().enqeue([this]()
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

    void ResourceEntry::unload_async()
    {
		std::mutex lock(mutex_);
        if (state_ == ResourceState::READY)
        {

        }
        ResourceState expected = ResourceState::READY;
        if (state_.compare_exchange_strong(expected, ResourceState::RELEASING))
        {
            
        }
    }

    void ResourceEntry::store_async(Callback on_done)
    {
        add_ref();

        JobSystem::get().enqeue([this, callback = std::move(on_done)]()
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

                const fs::path file_path = get_resource_path();

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
        return std::filesystem::exists(get_resource_path());
    }

    std::string ResourceEntry::get_name() const
    {
        const size_t pos = id_.find_last_of('/');
        return (pos == std::string::npos) ? id_ : id_.substr(pos + 1);
    }

    void ResourceEntry::set(Resource* resource)
    {
        Resource* old_resource = resource_.exchange(resource);
        delete old_resource;
    }

    std::string ResourceEntry::get_resource_path() const
    {
        return (std::filesystem::path(ResourceManager::get().get_assets_path()) / get_id()).string();
    }

    void ResourceEntry::update_async()
    {
        std::scoped_lock lock(mutex_);
		const ResourceState target_state = user_refs_ ? ResourceState::READY : ResourceState::NONE;
        if (target_state != state_)
        {
            add_ref();
            JobSystem::get().enqeue([this]()
                {
                    on_update_async();
                    remove_ref();
                });
        }
    }

    MappedResourceData ResourceEntry::get_mapped_data()
    {
		ResourceManager& manager = ResourceManager::get();
		return manager.get_mapped_data(*this);
    }
}

#endif