// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_serializer.h"

namespace cp
{
	BinarySerializer::BinarySerializer(void* data, uint64 size)
		: writing_(true)
		, input_stream_(nullptr, 0)
		, output_memory_view_(data, size)
		, output_stream_(output_memory_view_)
	{
	}

	BinarySerializer::BinarySerializer(const void* data, uint64 size)
		: writing_(false)
		, input_stream_(data, size)
		, output_memory_view_(nullptr, 0)
		, output_stream_(output_memory_view_)
	{
	}

	InputBinarySerializer::InputBinarySerializer(const void* data, uint64 size)
		: BinarySerializer(data, size)
	{
	}

	OutputBinarySerializer::OutputBinarySerializer(void* data, uint64 size)
		: BinarySerializer(data, size)
	{
	}

	bool BinarySerializer::failed() const
	{
		if (writing_)
			return output_stream_.failed();
		else
			return input_stream_.failed();
	}
}