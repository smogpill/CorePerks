// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_serializer.h"

namespace cp
{
	BinarySerializer::BinarySerializer(OutputMemory& output_memory, const void* data, uint64 size)
		: input_stream_(data, size)
		, output_stream_(output_memory)
	{
	}

	InputBinarySerializer::InputBinarySerializer(const void* data, uint64 size)
		: BinarySerializer(dummy_output_memory_, data, size)
	{
	}

	OutputBinarySerializer::OutputBinarySerializer(OutputMemory& memory)
		: BinarySerializer(memory, nullptr, 0)
	{
		writing_ = true;
	}

	bool BinarySerializer::failed() const
	{
		if (writing_)
			return output_stream_.failed();
		else
			return input_stream_.failed();
	}
}
