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
		mode_ = other.mode_;
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
        mode_ = mode;
#ifdef CP_WINDOWS
        DWORD desired_access = 0;
        DWORD creation_disposition = 0;
        DWORD share_mode = FILE_SHARE_READ;

        switch (mode)
        {
        case Mode::READ:
        {
			desired_access = GENERIC_READ;
            creation_disposition = OPEN_EXISTING;
            break;
        }
        case Mode::WRITE_TRUNCATE:
        {
			desired_access = GENERIC_WRITE;
            creation_disposition = CREATE_ALWAYS;
            share_mode = 0;
			break;
        }
        case Mode::WRITE_APPEND:
        {
			desired_access = GENERIC_WRITE | FILE_APPEND_DATA;
            creation_disposition = CREATE_ALWAYS;
            share_mode = 0;
            break;
        }
        default:
        {
            CP_ASSERT(false);
            break;
        }
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

    bool FileHandle::is_writable() const
    {
        return is_open() && (mode_ == Mode::WRITE_TRUNCATE || mode_ == Mode::WRITE_APPEND);
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

    bool FileHandle::write(const void* data, uint64 size)
    {
        CP_ASSERT(is_writable());
#ifdef CP_WINDOWS
        while (size > 0)
        {
            const DWORD chunk_size = static_cast<DWORD>(std::min(size, static_cast<uint64>(std::numeric_limits<DWORD>::max())));
			DWORD written;
            const BOOL success = WriteFile(native_handle_, data, chunk_size, &written, nullptr);
            if (!success || written != chunk_size)
                return false;
            data = static_cast<const uint8*>(data) + written;
			size -= written;
        }
        return true;
#else
        return false;
#endif
    }
}
