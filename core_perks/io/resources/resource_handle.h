// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class UntypedResourceHandle
	{
	public:
		UntypedResourceHandle() = default;
		UntypedResourceHandle(const std::string& id);
		//UntypedResourceHandle(const UntypedResourceHandle& other) : _entry(other._entry) {}

		void set_id(const std::string& id);
		void release() { _entry.release(); }
		void load_async(std::function<void(bool)> on_done = [](bool) {});
		void unload_async();
		void store_async(std::function<void(bool)> on_done = [](bool) {});
		auto get_name() const -> std::string { return _entry ? _entry->get_name() : nullptr; }
		auto get_id() const -> const std::string& { return _entry ? _entry->get_id() : nullptr; }
		//auto operator=(const UntypedResourceHandle& other) { _entry = other._entry; }
		operator bool() const { return _entry != nullptr; }

	protected:
		UntypedResourceHandle(ResourceEntry* entry) : _entry(entry) {}
		
		auto get() const -> Resource* { if (_entry) return _entry->get(); return nullptr; }
		void set_resource(Resource* resource);

		cp::RefPtr<ResourceEntry> _entry;
	};

	template <class T>
	class ResourceHandle : public UntypedResourceHandle
	{
		using Base = UntypedResourceHandle;
	public:
		using Base::UntypedResourceHandle;
		using Base::operator=;
		/*
		ResourceHandle() = default;
		ResourceHandle(const std::string& id);
		ResourceHandle(const ResourceHandle& other) : _entry(other._entry) {}
		*/

		//void ReloadAsync();
		auto get() const -> T* { return static_cast<T*>(Base::get()); }
		void set_resource(T* resource) { Base::set_resource(resource); }
		//auto operator=(const ResourceHandle& other) -> ResourceHandle& { _entry = other._entry; return *this; }

	private:
		friend class ResourceManager;
		friend class ResourceLoader;
	};
}
