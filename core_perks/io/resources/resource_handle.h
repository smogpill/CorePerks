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
		ResourceHandle(const ResourceID& id, const Type& type);

		void bind(const ResourceID& id, const Type& type);
		void reset();

		operator bool() const { return entry_ != nullptr; }
		ResourceEntry* operator->() const { CP_ASSERT(entry_); return entry_.get(); }
		ResourceEntry& operator*() const { CP_ASSERT(entry_); return *entry_.get(); }
		bool operator==(const ResourceHandle& other) const { return entry_ == other.entry_; }
		bool operator!=(const ResourceHandle& other) const { return entry_ != other.entry_; }

	protected:
		friend class ResourceManager;
		friend class ResourceEntry;
		friend class Resource;
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
		ResourceHandleT() = default;
		ResourceHandleT(const ResourceID& id) : Base(id, T::get_type_static()) {}
		ResourceHandleT(const ResourceHandleT& handle) = default;
		ResourceHandleT(const ResourceHandle& handle) : entry_(handle.entry_) {}
		ResourceHandleT(ResourceHandle&& handle) : entry_(std::move(handle.entry_)) {}
		using ResourceHandle::operator=;

		void bind(const ResourceID& id) { Base::bind(id, T::get_type_static()); }
		RefPtr<T> get() const { return entry_ ? RefPtr<T>(std::move(entry_->get())) : RefPtr<T>(); }
	};
}
