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
		auto data() { return _data; }
		auto size() const { return _size; }
		auto span() -> std::span<std::byte> { return std::span<std::byte>(static_cast<std::byte*>(_data), _size); }
		bool is_mapped() const { return _data != nullptr; }
		auto begin() -> std::byte* { return static_cast<std::byte*>(_data); }
		auto end() -> std::byte* { return static_cast<std::byte*>(_data) + _size; }

	private:
		friend class FileHandle;

		void* _data = nullptr;
		uint64 _size = 0;
#ifdef CP_WINDOWS
		HANDLE _mapping = INVALID_HANDLE_VALUE;
		void* _view = nullptr;
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
		auto map(Access access = Access::READ_ONLY) -> MappedRegion;
		auto map_region(uint64 offset, uint64 size, Access access = Access::READ_ONLY) -> MappedRegion;
		auto get_size() const -> uint64;
		auto native_handle() const { return _native_handle; }

	private:
		void open(const std::string& path, Mode mode);
		void close();

		std::string _path;

#ifdef CP_WINDOWS
		HANDLE _native_handle = INVALID_HANDLE_VALUE;
#endif
	};
}
