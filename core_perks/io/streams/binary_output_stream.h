// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream
	{
	public:
		BinaryOutputStream();
		~BinaryOutputStream();
		void write(const void* buffer, uint64 size);
		template <class T>
		void write(const T& value);
		uint64 size() const;
		void output_to_buffer(void* dest_buffer);

	private:
		static constexpr uint32 block_size = 64 * 1024;
		std::vector<uint8*> blocks_;
		uint32 current_block_pos_ = 0;
	};

	template <class T>
	CP_FORCE_INLINE void BinaryOutputStream::write(const T& value)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		write(&value, sizeof(T));
	}
}
