// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Resource; class BinaryInputStream; class BinaryOutputStream;
	class Type; class HashedString;

	class ResourceHandleBase
	{
	public:
		ResourceHandleBase() = default;
		~ResourceHandleBase() = default;

		void load_async(std::function<void()> on_done = [](){});
		void release() { resource_.release(); }
		std::string get_name() const;
		const HashedString& get_id() const;
		//auto operator=(const ResourceHandle& other) { entry_ = other.entry_; }
		operator bool() const { return resource_ != nullptr; }

	protected:
		friend class ResourceManager;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle);

		RefPtr<Resource> resource_;
	};

	template <class T = Resource>
	class ResourceHandle : public ResourceHandleBase
	{
		CP_BASE(ResourceHandleBase);
	public:
		ResourceHandle() = default;
		ResourceHandle(const HashedString& id) : Base(id, T::get_type_static()) {}
		void set(const HashedString& id) { set(id, T::get_type_static()); }
		T* get() const;
	};
}
