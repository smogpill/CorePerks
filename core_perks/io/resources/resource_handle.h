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
		ResourceHandleBase(Resource* resource);
		~ResourceHandleBase() = default;

		void release() { resource_.release(); }
		ResourceHandleBase& operator=(Resource* resource) { resource_ = resource; }
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
		T* get() const;
		using operator=;

	private:
		friend class ResourceManager;
		explicit ResourceHandle(Resource* resource) { resource_ = resource; }
	};
}
