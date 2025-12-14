// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
//#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class ResourceEntry;

	class UntypedResourceHandle
	{
	public:
		UntypedResourceHandle() = default;
		UntypedResourceHandle(const std::string& id, const Type& type);
		//UntypedResourceHandle(const UntypedResourceHandle& other) : entry_(other.entry_) {}

		void release() { entry_.release(); }
		void load_async(std::function<void(bool)> on_done = [](bool) {});
		void unload_async();
		void store_async(std::function<void(bool)> on_done = [](bool) {});
		std::string get_name() const { return entry_ ? entry_->get_name() : nullptr; }
		const std::string& get_id() const { return entry_ ? entry_->get_id() : nullptr; }
		//auto operator=(const UntypedResourceHandle& other) { entry_ = other.entry_; }
		operator bool() const { return entry_ != nullptr; }

	protected:
		UntypedResourceHandle(ResourceEntry* entry) : entry_(entry) {}
		
		void set_id(const std::string& id, const Type& type);
		Resource* get() const { return entry_ ? entry_->get() : nullptr; }
		void set_resource(Resource* resource);

		cp::RefPtr<ResourceEntry> entry_;
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
		ResourceHandle(const ResourceHandle& other) : entry_(other.entry_) {}
		*/

		//void ReloadAsync();
		T* get() const { return static_cast<T*>(Base::get()); }
		void set_resource(T* resource) { Base::set_resource(resource); }
		//auto operator=(const ResourceHandle& other) -> ResourceHandle& { entry_ = other.entry_; return *this; }

	private:
		friend class ResourceManager;
		friend class ResourceLoader;
	};
}
