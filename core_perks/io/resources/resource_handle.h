// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class BinaryInputStream; class BinaryOutputStream; class Type; class ResourceID;
	class ResourceEntry;

	class ResourceHandle
	{
	public:
		ResourceHandle() = default;
		void release();
		operator bool() const { return entry_ != nullptr; }
		ResourceEntry* operator->() const { CP_ASSERT(entry_); return entry_.get(); }
		ResourceEntry& operator*() const { CP_ASSERT(entry_); return *entry_.get(); }

	protected:
		friend class ResourceManager;
		friend class ResourceEntry;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle);

		ResourceHandle(ResourceEntry* entry);

		RefPtr<ResourceEntry> entry_;
	};

	template <class T>
	class ResourceHandleT : public ResourceHandle
	{
		CP_BASE(ResourceHandle);
	public:
		using ResourceHandle::ResourceHandle;
		using ResourceHandle::operator=;

		T* get() const { return entry_ ? entry_->get<T>() : nullptr; }
	};
}
