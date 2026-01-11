// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/memory/memory.h"

namespace cp
{
	class OutputMemory
	{
	public:
		void write(const void* buffer, uint64 size);
		virtual uint64 size() const = 0;
		virtual bool failed() const { return false; }

	protected:
		friend class BinaryOutputStream;
		virtual void grow(uint64 size) = 0;

		uint8* ptr_ = nullptr;
		uint64 capacity_ = 0;
	};

	class OutputMemoryView : public OutputMemory
	{
	public:
		OutputMemoryView(void* buffer, uint64 size);

		uint64 size() const override { return ptr_ - buffer_; }
		bool failed() const override { return failed_; }
		
	protected:
		void grow(uint64 size) override { failed_ = true; }

	private:
		uint8* buffer_ = nullptr;
		bool failed_ = false;
	};

	class OutputMemoryBuffer : public OutputMemory
	{
	public:
		~OutputMemoryBuffer();

		void copy_to_buffer(void* dest_buffer);
		uint64 size() const override;

	protected:
		void grow(uint64 size) override;

	private:
		static constexpr uint32 block_size = 16 * page_size;
		std::vector<uint8*> blocks_;
	};

	class BinaryOutputStream
	{
	public:
		BinaryOutputStream(OutputMemory& memory) : memory_(memory) {}
		CP_FORCE_INLINE void write(const void* buffer, uint64 size) { memory_.write(buffer, size); }
		template <class T> requires std::is_trivially_copyable_v<T>
		CP_FORCE_INLINE void write(const T& value) { memory_.write(&value, sizeof(T)); }
		uint64 size() const { return memory_.size(); }
		bool failed() const { return memory_.failed(); }

	private:
		OutputMemory& memory_;
	};

	template <class T> requires std::is_trivially_copyable_v<T>
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const T& value) { stream.write(value); return stream; }
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const std::string& str);

	template <class T>
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const std::vector<T>& v)
	{
		const uint32 size = (uint32)v.size();
		stream << size;
		if (size > 0)
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				stream.write(v.data(), size * sizeof(T));
			}
			else
			{
				for (uint32 i = 0; i < size; ++i)
					stream << v[i];
			}
		}

		return stream;
	}
}
