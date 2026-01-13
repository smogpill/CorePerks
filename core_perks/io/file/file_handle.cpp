// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/file/file_handle.h"

namespace cp
{
    FileHandle::FileHandle(const std::string& path, Mode mode)
	{
        open(path, mode);
	}

    FileHandle::FileHandle(FileHandle&& other)
    {
#ifdef CP_WINDOWS
        native_handle_ = other.native_handle_;
        other.native_handle_ = INVALID_HANDLE_VALUE;
#endif
#ifdef CP_DEBUG_OR_DEV
        debug_path_ = std::move(other.debug_path_);
#endif
    }

    FileHandle::~FileHandle()
	{
		close();
	}

	bool FileHandle::open(const std::string& path, Mode mode)
	{
        close();
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
        native_handle_ = CreateFileW(wpath.data(), desired_access, share_mode, nullptr, creation_disposition, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (native_handle_ == INVALID_HANDLE_VALUE)
            return false;
#endif
#ifdef CP_DEBUG_OR_DEV
        debug_path_ = path;
#endif
        return true;
	}

	void FileHandle::close()
	{
		if (is_open())
		{
#ifdef CP_WINDOWS
			CloseHandle(native_handle_);
#endif
#ifdef CP_DEBUG_OR_DEV
            debug_path_.clear();
#endif
		}
	}

	bool FileHandle::is_open() const
	{
#ifdef CP_WINDOWS
		return native_handle_ != INVALID_HANDLE_VALUE;
#endif
	}

    uint64 FileHandle::get_size() const
	{
		if (!is_open())
			return 0;
#ifdef CP_WINDOWS
		LARGE_INTEGER size;
		if (!GetFileSizeEx(native_handle_, &size))
			return 0;
		return static_cast<uint64>(size.QuadPart);
#endif
	}
}
