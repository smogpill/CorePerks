// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/base.h"
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/io/file/file_handle.h"

namespace cp
{
    MappedFileRegion::MappedFileRegion(const RefPtr<FileHandle>& file, Access access)
        : MappedFileRegion(file, 0, 0, access)
    {

    }

    MappedFileRegion::MappedFileRegion(const RefPtr<FileHandle>& file, uint64 offset, uint64 requested_size, Access access)
        : file_(file)
    {
        if (!file_->is_open())
            return;

        const uint64 file_size = file_->get_size();
        if (offset + requested_size > file_size)
        {
            CP_ERROR("Can't map file region for file {}: region is not within bounds: [offset: {}, size: {}] while file size is {}", file_->get_path(), offset, requested_size, file_size);
            return;
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
        DWORD size_low = static_cast<DWORD>(requested_size & 0xFFFFFFFF);
        DWORD size_high = static_cast<DWORD>((requested_size >> 32) & 0xFFFFFFFF);

        mapping_ = CreateFileMappingW(file_->get_native_handle(), nullptr, protect, size_high, size_low, nullptr);
        if (!mapping_)
        {
            CP_ERROR("Failed to map file: {}", file_->get_path());
            return;
        }

        // Map view
        DWORD offset_low = static_cast<DWORD>(offset & 0xFFFFFFFF);
        DWORD offset_high = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        data_ = MapViewOfFile(mapping_, desired_access, offset_high, offset_low, static_cast<SIZE_T>(requested_size));
        if (!data_)
        {
            DWORD err = GetLastError();
            CP_ERROR("Failed to map view of file: {}", file_->get_path());
            return;
        }

        size_ = requested_size;
#endif
    }

    MappedFileRegion::~MappedFileRegion()
    {
#ifdef CP_WINDOWS
        if (data_)
            UnmapViewOfFile(data_);
        if (mapping_ != INVALID_HANDLE_VALUE)
            CloseHandle(mapping_);
#endif
    }

    void* MappedFileRegion::data()
    {
        CP_ASSERT(access_ == Access::READ_WRITE);
        return data_;
    }
}
