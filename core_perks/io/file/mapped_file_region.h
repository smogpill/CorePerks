// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class FileHandle;

	class MappedFileRegion
	{
	public:
		enum class Access
		{
			READ_ONLY,
			READ_WRITE
		};
		MappedFileRegion();
		MappedFileRegion(const RefPtr<FileHandle>& file, Access access = Access::READ_ONLY);
		/// If the size is 0, the size is automatically set to match the end of the file.
		MappedFileRegion(const RefPtr<FileHandle>& file, uint64 offset, uint64 size, Access access = Access::READ_ONLY);
		MappedFileRegion(MappedFileRegion&& other);
		~MappedFileRegion();

		const void* data() const { return data_; }
		void* data();
		uint64 size() const { return size_; }
		std::span<uint8> span() { return std::span<uint8>(static_cast<uint8*>(data_), size_); }
		bool is_mapped() const { return data_ != nullptr; }
		uint8* begin() { return static_cast<uint8*>(data_); }
		uint8* end() { return static_cast<uint8*>(data_) + size_; }
		MappedFileRegion& operator=(MappedFileRegion&& other);

	private:
		void* data_ = nullptr;
		uint64 size_ = 0;
		RefPtr<FileHandle> file_;
		Access access_ = Access::READ_ONLY;
#ifdef CP_WINDOWS
		HANDLE native_handle_ = INVALID_HANDLE_VALUE;
#endif
	};
}
