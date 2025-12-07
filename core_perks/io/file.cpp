// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/file.h"

namespace cp
{
    MappedRegion::~MappedRegion()
    {
#ifdef CP_WINDOWS
        if (_view)
            UnmapViewOfFile(_view);
        if (_mapping)
            CloseHandle(_mapping);
#endif
    }

    FileHandle::FileHandle(const std::string& path, Mode mode)
	{
        open(path, mode);
	}

    FileHandle::FileHandle(FileHandle&& other)
        : _path(other._path)
    {
#ifdef CP_WINDOWS
        _native_handle = other._native_handle;
        other._native_handle = INVALID_HANDLE_VALUE;
#endif
    }

    FileHandle::~FileHandle()
	{
		close();
	}

	void FileHandle::open(const std::string& path, Mode mode)
	{
#ifdef CP_WINDOWS
        DWORD desired_access = 0;
        DWORD creation_disposition = 0;
        DWORD share_mode = FILE_SHARE_READ;

        uint32 mode_flags = static_cast<uint32>(mode);

        // Access
        if (mode_flags & static_cast<uint32_t>(Mode::READ))
            desired_access |= GENERIC_READ;
        if (mode_flags & static_cast<uint32_t>(Mode::WRITE))
            desired_access |= GENERIC_WRITE;
        if (mode_flags & static_cast<uint32_t>(Mode::APPEND))
            desired_access |= FILE_APPEND_DATA;

        // Creation
        if (mode_flags & static_cast<uint32_t>(Mode::CREATE))
        {
            if (mode_flags & static_cast<uint32_t>(Mode::EXCLUSIVE))
                creation_disposition = CREATE_NEW;
            else if (mode_flags & static_cast<uint32_t>(Mode::TRUNCATE))
                creation_disposition = CREATE_ALWAYS;
            else
                creation_disposition = OPEN_ALWAYS;
        }
        else if (mode_flags & static_cast<uint32_t>(Mode::TRUNCATE))
        {
            creation_disposition = TRUNCATE_EXISTING;
        }
        else 
        {
            creation_disposition = OPEN_EXISTING;
        }

        // Open
        std::wstring wpath(path.begin(), path.end());
        _native_handle = CreateFileW(wpath.data(), desired_access, share_mode, nullptr, creation_disposition, FILE_ATTRIBUTE_NORMAL, nullptr);
#endif
	}

	void FileHandle::close()
	{
		if (is_open())
		{
#ifdef CP_WINDOWS
			CloseHandle(_native_handle);
#endif
		}
	}

	bool FileHandle::is_open() const
	{
#ifdef CP_WINDOWS
		return _native_handle != INVALID_HANDLE_VALUE && _native_handle != nullptr;
#endif
	}

	auto FileHandle::map(Access access) -> MappedRegion
	{
		return map_region(0, get_size(), access);
	}

	auto FileHandle::map_region(uint64 offset, uint64 size, Access access) -> MappedRegion
	{
		MappedRegion region;
		if (!is_open())
			return region;


		const uint64 file_size = get_size();
		if (offset + size > file_size)
		{
			CP_ERROR("Can't map file region for file {}: region is not within bounds: [offset: {}, size: {}] while file size is {}", _path, offset, size, file_size);
			return region;
		}

#ifdef CP_WINDOWS

        // Convert access flags
        DWORD protect = 0;
        DWORD desired_access = 0;

        if (access == Access::READ_ONLY)
        {
            protect = PAGE_READONLY;
            desired_access = FILE_MAP_READ;
        }
        else
        {
            protect = PAGE_READWRITE;
            desired_access = FILE_MAP_WRITE;
        }

        // Create file mapping
        DWORD size_low = static_cast<DWORD>(size & 0xFFFFFFFF);
        DWORD size_high = static_cast<DWORD>((size >> 32) & 0xFFFFFFFF);

        HANDLE mapping = CreateFileMappingW(_native_handle, nullptr, protect, size_high, size_low, nullptr);

        if (!mapping)
		{
			CP_ERROR("Failed to map file: {}", _path);
			return region;
        }

        // Map view
        DWORD offset_low = static_cast<DWORD>(offset & 0xFFFFFFFF);
        DWORD offset_high = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        void* view = MapViewOfFile(mapping, desired_access, offset_high, offset_low, static_cast<SIZE_T>(size));

        if (!view) 
		{
            DWORD err = GetLastError();
            CloseHandle(mapping);
			CP_ERROR("Failed to map view of file: {}", _path);
			return region;
        }

        region._data = view;
        region._size = size;
        region._mapping = mapping;
        region._view = view;
#endif

		return region;
	}

	auto FileHandle::get_size() const -> uint64
	{
		if (!is_open())
			return 0;
#ifdef CP_WINDOWS
		LARGE_INTEGER size;
		if (!GetFileSizeEx(_native_handle, &size))
			return 0;
		return static_cast<uint64>(size.QuadPart);
#endif
	}
}
