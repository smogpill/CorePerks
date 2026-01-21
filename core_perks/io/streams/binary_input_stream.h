// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryInputStream
	{
	public:
		BinaryInputStream(const void* data, uint64 size);
		BinaryInputStream(const void* data);

		void read(void* buffer, uint64 size);
		template <class T> requires std::is_trivially_copyable_v<T>
		void read(T& value);
		bool failed() const { return failed_; }
		void set_failed();

	private:
		const uint8* data_ = nullptr;
		const uint8* data_end_ = nullptr;
		bool failed_ = false;
	};

	CP_FORCE_INLINE void BinaryInputStream::read(void* buffer, uint64 size)
	{
		CP_ASSERT(buffer);
		const uint8* next_data = data_ + size;
		if (next_data <= data_end_) [[likely]]
		{
			std::memcpy(buffer, data_, size);
			data_ = next_data;
		}
		else
		{
			set_failed();
		}
	}

	template <class T> requires std::is_trivially_copyable_v<T>
	CP_FORCE_INLINE void BinaryInputStream::read(T& value)
	{
		read(&value, sizeof(T));
	}

	template <class T> requires std::is_trivially_copyable_v<T>
	BinaryInputStream& operator>>(BinaryInputStream& stream, T& value) { stream.read(value); return stream; }
	BinaryInputStream& operator>>(BinaryInputStream& stream, std::string& str);

	template <class T>
	BinaryInputStream& operator>>(BinaryInputStream& stream, std::vector<T>& v)
	{
		uint32 size = 0;
		stream >> size;
		v.resize(size);
		if (size > 0)
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				stream.read(v.data(), size * sizeof(T));
			}
			else
			{
				for (uint32 i = 0; i < size; ++i)
					stream >> v[i];
			}
		}
		return stream;
	}
}
