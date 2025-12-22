// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class MappedRegion
	{
	public:
		~MappedRegion();
		void* data() { return data_; }
		uint64 size() const { return size_; }
		auto span() -> std::span<uint8> { return std::span<uint8>(static_cast<uint8*>(data_), size_); }
		bool is_mapped() const { return data_ != nullptr; }
		uint8* begin() { return static_cast<uint8*>(data_); }
		uint8* end() { return static_cast<uint8*>(data_) + size_; }

	private:
		friend class FileHandle;

		void* data_ = nullptr;
		uint64 size_ = 0;
		FileHandle* file_ = nullptr;
#ifdef CP_WINDOWS
		HANDLE mapping_ = INVALID_HANDLE_VALUE;
		void* view_ = nullptr;
#endif
	};

	class FileHandle
	{
	public:
		enum class Mode
		{
			READ = 1 << 0,
			WRITE = 1 << 1,
			APPEND = 1 << 2,
			CREATE = 1 << 3,
			TRUNCATE = 1 << 4,
			EXCLUSIVE = 1 << 5
		};
		enum class Access
		{
			READ_ONLY,
			READ_WRITE
		};
		FileHandle() = default;
		FileHandle(const std::string& path, Mode mode = Mode::READ);
		FileHandle(FileHandle&& other);
		~FileHandle();
		
		bool is_open() const;
		uint64 get_size() const;
		MappedRegion map(Access access = Access::READ_ONLY);
		MappedRegion map_region(uint64 offset, uint64 size, Access access = Access::READ_ONLY);
		auto native_handle() const { return native_handle_; }
		uint32 get_mapped_region_count() const { return mapped_region_count_; }

	private:
		friend class MappedRegion;
		void open(const std::string& path, Mode mode);
		void close();
		void on_mapped_region_unmapped();

		std::string path_;
		std::atomic<uint32> mapped_region_count_ = 0;

#ifdef CP_WINDOWS
		HANDLE native_handle_ = INVALID_HANDLE_VALUE;
#endif
	};
}
