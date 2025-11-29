// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class ResourceHandleBase
	{
	public:
		ResourceHandleBase() = default;
		ResourceHandleBase(const std::string& id);
		ResourceHandleBase(const ResourceHandleBase& other) : _holder(other._holder) {}

		void set_id(const std::string& id);
		void release() { _holder.release(); }
		void add_load_callback(std::function<void(bool)> callback) { CP_ASSERT(_holder); _holder->add_load_callback(callback); }
		void unload_async();
		void store_async(std::function<void(bool)> onDone = [](bool) {});
		auto get_name() const -> std::string { return _holder ? _holder->get_name() : nullptr; }
		auto get_id() const -> const std::string& { return _holder ? _holder->get_id() : nullptr; }
		auto operator=(const ResourceHandleBase& other) { _holder = other._holder; }
		operator bool() const { return _holder != nullptr; }

	protected:
		ResourceHandleBase(ResourceHolder* holder) : _holder(holder) {}
		void load_async(std::function<Resource* ()> createFunction);
		auto get() const -> Resource* { if (_holder) return _holder->get(); return nullptr; }
		void set_resource(Resource* resource);

		cp::RefPtr<ResourceHolder> _holder;
	};

	template <class T>
	class ResourceHandle : public ResourceHandleBase
	{
		using Base = ResourceHandleBase;
	public:
		using Base::ResourceHandleBase;
		using Base::operator=;
		/*
		ResourceHandle() = default;
		ResourceHandle(const std::string& id);
		ResourceHandle(const ResourceHandle& other) : _holder(other._holder) {}
		*/

		//void ReloadAsync();
		auto get() const -> T* { return static_cast<T*>(Base::get()); }
		void load_async() { Base::load_async([]() { return new T(); }); }
		void set_resource(T* resource) { Base::set_resource(resource); }
		auto operator=(const ResourceHandle& other) -> ResourceHandle& { _holder = other._holder; return *this; }

	private:
		friend class ResourceManager;
		friend class ResourceLoader;
	};
}
