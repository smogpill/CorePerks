// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class FileHandle : public RefCounted
	{
	public:
		enum class Mode
		{
			READ,
			WRITE_TRUNCATE,
			WRITE_APPEND
		};

		FileHandle() = default;
		FileHandle(const std::string& path, Mode mode = Mode::READ);
		FileHandle(FileHandle&& other);
		~FileHandle();
		
		bool open(const std::string& path, Mode mode = Mode::READ);
		void close();
		bool is_open() const;
		bool is_writable() const;
		uint64 get_size() const;
		bool write(const void* data, uint64 size);
		const auto& get_native_handle() const { return native_handle_; }
#ifdef CP_DEBUG_OR_DEV
		const std::string& get_debug_path() const { return debug_path_; }
#endif

	private:
		Mode mode_ = Mode::READ;
#ifdef CP_WINDOWS
		HANDLE native_handle_ = INVALID_HANDLE_VALUE;
#endif
#ifdef CP_DEBUG_OR_DEV
		std::string debug_path_;
#endif
	};
}
