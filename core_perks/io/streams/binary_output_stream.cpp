// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_output_stream.h"

namespace cp
{
	BinaryOutputStream::BinaryOutputStream()
	{
		blocks_.push_back(new uint8[block_size]);
	}

	BinaryOutputStream::~BinaryOutputStream()
	{
		for (uint8* block : blocks_)
			delete[] block;
	}

	void BinaryOutputStream::write(const void* buffer, uint64 size)
	{
		const uint8* src = static_cast<const uint8*>(buffer);
		while (size > 0) [[likely]]
		{
			if (current_block_pos_ == block_size) [[unlikely]]
			{
				blocks_.push_back(new uint8[block_size]);
				current_block_pos_ = 0;
			}
			const uint64 available = block_size - current_block_pos_;
			const uint64 to_write = size <= available ? size : available;
			std::memcpy(blocks_.back() + current_block_pos_, src, to_write);
			src += to_write;
			current_block_pos_ += static_cast<uint32>(to_write);
			size -= to_write;
		}
	}

	uint64 BinaryOutputStream::size() const
	{
		if (blocks_.empty())
			return 0;
		return static_cast<uint64>(blocks_.size() - 1) * block_size + current_block_pos_;
	}

	void BinaryOutputStream::output_to_buffer(void* dest_buffer)
	{
		uint8* dest = static_cast<uint8*>(dest_buffer);
		const uint block_count_minus_one = (uint)blocks_.size() - 1;
		for (uint block_idx = 0; block_idx < block_count_minus_one; ++block_idx)
		{
			const uint8* block = blocks_[block_idx];
			std::memcpy(dest, block, block_size);
		}

		if (current_block_pos_)
			std::memcpy(dest, blocks_.back(), current_block_pos_);
	}
}
