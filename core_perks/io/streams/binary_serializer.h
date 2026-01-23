// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/io/streams/binary_input_stream.h"
#include "core_perks/io/streams/binary_output_stream.h"

namespace cp
{
	/// Handles:
	/// - Writing code once for both serialization and deserialization operations.
	/// - Conditional operations such as versioning and removals of serialized values. 
	/// 
	/// Inspired by Media Molecule's "LBP" serialization method:
	/// https://handmade.network/p/29/swedish-cubes-for-unity/blog/p/2723-how_media_molecule_does_serialization
	class BinarySerializer
	{
	public:
		bool failed() const;
		
		uint32 version_ = 0;
		bool writing_ = false;
		BinaryInputStream input_stream_;
		BinaryOutputStream output_stream_;

	protected:
		BinarySerializer(const void* data, uint64 size);
		BinarySerializer(void* data, uint64 size);

	private:
		OutputMemoryView output_memory_view_;
	};

	class InputBinarySerializer : public BinarySerializer
	{
	public:
		InputBinarySerializer(const void* data, uint64 size);
	};

	class OutputBinarySerializer : public BinarySerializer
	{
	public:
		OutputBinarySerializer(void* data, uint64 size);
	};


	template <class T>
	struct Versioned
	{
		CP_FORCE_INLINE Versioned(uint32 version, T& value) : value_(value), version_(version) {}
		T& value_;
		uint32 version_;
	};

	template <class T>
	CP_FORCE_INLINE Versioned<T> versioned(uint32 version, T& value) { return Versioned(version, value); }

	template <class T>
	struct Removed
	{
		CP_FORCE_INLINE Removed(uint32 added_version, uint32 removed_version, const char* name, const T& value = T())
			: added_version_(added_version), removed_version_(removed_version), value_(value) { }
		uint32 added_version_;
		uint32 removed_version_;
		T value_;
	};

	template <class T>
	CP_FORCE_INLINE Removed<T> removed(uint32 added_version, uint32 removed_version, const char* name, const T& value = T()) { return Removed<T>(added_version, removed_version, name, value); }

	template <class T> 
	BinarySerializer& operator/(BinarySerializer& serializer, T& value)
	{
		if (serializer.writing_)
			serializer.output_stream_ << value;
		else
			serializer.input_stream_ >> value;
		return serializer;
	}

	template <class T>
	BinarySerializer& operator/(BinarySerializer& serializer, Versioned<T> versioned)
	{
		if (serializer.version_ >= versioned.version_)
			serializer / versioned.value_;
		return serializer;
	}

	template <class T>
	BinarySerializer& operator/(BinarySerializer& serializer, Removed<T> removed)
	{
		if (serializer.version_ >= removed.added_version_ && serializer.version_ < removed.removed_version_)
			serializer / removed.value_;
		return serializer;
	}
}
