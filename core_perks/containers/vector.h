// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/memory/allocators.h"

namespace cp
{
	template <class T>
	class Vector
	{
	public:
		Vector() = default;
		explicit Vector(AllocTag tag) : tag_(tag) {}
		explicit Vector(uint32 initial_size, AllocTag tag = AllocTag());
		Vector(uint32 initial_size, const T& value, AllocTag tag = AllocTag());
		Vector(const Vector<T>& other, AllocTag tag = AllocTag());
		Vector(Vector<T>&& other);
		Vector(Vector<T>&& other, AllocTag tag);
		Vector(std::initializer_list<T> list, AllocTag tag = AllocTag());
		~Vector();

		void clear();
		void delete_and_clear();
		void reverse_delete_and_clear();
		void reserve(uint32 size);
		void resize(uint32 size);
		void resize(uint32 size, const T& value);
		void push_back(const T& value);
		void push_back(T&& value);
		template <class... Args>
		T& emplace_back(Args&&... args);
		void append_range(const Vector<T>& range);
		void append_range(const std::span<T>& range);
		const T& pop_back() { CP_ASSERT(size_); return data_[--size_]; }
		void pop_front(uint32 count);
		T* begin() { return data_; }
		const T* begin() const { return data_; }
		const T* cbegin() const { return data_; }
		T* end() { return data_ + size_; }
		const T* end() const { return data_ + size_; }
		const T* cend() const { return data_ + size_; }
		T* data() { return data_; }
		const T* data() const { return data_; }
		T& back() { CP_ASSERT(size_); return data_[size_ - 1]; }
		const T& back() const { CP_ASSERT(size_); return data_[size_ - 1]; }
		T& operator[] (uint32 i) { CP_ASSERT(i < size_); return data_[i]; }
		const T& operator[] (uint32 i) const { CP_ASSERT(i < size_); return data_[i]; }
		void operator=(const Vector<T>& other);
		constexpr void operator=(Vector<T>&& other);
		constexpr void operator=(std::initializer_list<T> list);
		bool operator==(const Vector<T>& other) const;
		bool operator!=(const Vector<T>& other) const { return !operator==(other); }
		bool empty() const { return size_ == 0; }
		uint32 size() const { return size_; }
		uint32 capacity() const { return capacity_; }
		void swap(Vector<T>& other) { swap_memory(*this, other); }
		void shrink_to_fit();
		T* erase(T* it);
		const T* erase(const T* it) { return (const T*)erase((T*)it); }
		template <class U>
		void erase_first(const U& value);
		uint32 erase_at(uint32 index);
		void erase_before(uint32 end_idx);
		T* swap_and_pop(T* it);
		const T* swap_and_pop(const T* it) { return (const T*)swap_and_pop((T*)it); }
		template <class U>
		void swap_and_pop_first(const U& value);
		uint32 swap_and_pop_at(uint32 index);
		template <class U>
		bool contains(const U& value) const;
		template <class F>
		bool contains_if(F func) const;
		template <class U>
		void fill(const U& value);

	private:
		void set_capacity(uint32 new_capacity);

		T* data_ = nullptr;
		uint32 size_ = 0;
		uint32 capacity_ = 0;
		AllocTag tag_ = 0;
	};


	uint32 compute_best_Vector_capacity(uint32 desired_capacity);

	template <class T>
	Vector<T>::Vector(uint32 initial_size, AllocTag tag)
		: tag_(tag)
	{
		resize(initial_size);
	}

	template <class T>
	Vector<T>::Vector(uint32 initial_size, const T& value, AllocTag tag)
		: tag_(tag)
	{
		resize(initial_size, value);
	}

	template <class T>
	Vector<T>::Vector(const Vector<T>& other, AllocTag tag)
		: tag_(tag)
	{
		operator=(other);
	}

	template <class T>
	Vector<T>::Vector(Vector<T>&& other)
		: data_(other.data_)
		, size_(other.size_)
		, capacity_(other.capacity_)
		, tag_(other.tag)
	{
		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	template <class T>
	Vector<T>::Vector(Vector<T>&& other, AllocTag tag)
		: tag_(tag)
	{
		if (tag_ == other.tag_)
			operator=(std::move(other));
		else
			operator=(other);
	}

	template <class T>
	Vector<T>::Vector(std::initializer_list<T> list, AllocTag tag)
		: tag_(tag)
	{
		operator=(list);
	}

	template <class T>
	Vector<T>::~Vector()
	{
		if (data_)
		{
			std::destroy(data_, data_ + size_);
			free(data_);
		}
	}

	template <class T>
	void Vector<T>::clear()
	{
		if (!size_)
			return;
		std::destroy(data_, data_ + size_);
#ifdef CP_DEBUG
		mark_as_deleted(data_, uint64(size_) * sizeof(T));
#endif
		size_ = 0;
	}

	template <class T>
	void Vector<T>::delete_and_clear()
	{
		for (T p : *this)
			delete p;
		clear();
	}

	template<class T>
	void Vector<T>::reverse_delete_and_clear()
	{
		for (uint32 i = size_; i--;)
			delete data_[i];
		clear();
	}

	template <class T>
	void Vector<T>::set_capacity(uint32 new_capacity)
	{
		CP_ASSERT(new_capacity >= size_);
		if (new_capacity != capacity_)
		{
			// Aligned to min 16 bytes:
			// - For simplicity. For example for working using SIMD on a float array.
			// - Seems faster on Intel architectures https://software.intel.com/en-us/articles/data-alignment-when-migrating-to-64-bit-intel-architecture).
			const uint alignment = alignof(T) > 16 ? alignof(T) : 16;
			T* new_buffer;
			const size_t new_capacity8 = uint64(new_capacity) * sizeof(T);
			new_buffer = static_cast<T*>(aligned_alloc(alignment, new_capacity8, tag_));
			if (data_)
			{
				std::uninitialized_move(data_, data_ + size_, new_buffer);
				std::destroy(data_, data_ + size_);
				free(data_);
			}
			data_ = new_buffer;
			capacity_ = new_capacity;
		}
	}

	template <class T>
	CP_FORCE_INLINE void Vector<T>::reserve(uint32 size)
	{
		if (size > capacity_)
			set_capacity(compute_best_Vector_capacity(size));
	}

	template <class T>
	void Vector<T>::resize(uint32 new_size)
	{
		if (new_size != size_)
		{
			if (new_size > size_)
			{
				reserve(new_size);
				::new (data_ + size_) T[new_size - size_];
			}
			else
			{
				std::destroy(data_ + new_size, data_ + size_);
#ifdef CP_DEBUG
				mark_as_deleted(data_ + new_size, uint64(size_ - new_size) * sizeof(T));
#endif
			}
			size_ = new_size;
		}
	}

	template <class T>
	void Vector<T>::resize(uint32 new_size, const T& value)
	{
		if (new_size != size_)
		{
			if (new_size > size_)
			{
				reserve(new_size);
				for (uint32 i = size_; i < new_size; ++i)
					::new (data_ + i) T(value);
			}
			else
			{
				std::destroy(data_ + new_size, data_ + size_);
#ifdef CP_DEBUG
				mark_as_deleted(data_ + new_size, uint64(size_ - new_size) * sizeof(T));
#endif
			}
			size_ = new_size;
		}
	}

	template <class T>
	void Vector<T>::push_back(const T& value)
	{
		emplace_back(value);
	}

	template <class T>
	void Vector<T>::push_back(T&& value)
	{
		emplace_back(std::move(value));
	}

	template <class T>
	template <class... Args>
	T& Vector<T>::emplace_back(Args&&... args)
	{
		const uint32 new_size = size_ + 1;
		reserve(new_size);
		T& e = data_[size_];
		new (&e) T(std::forward<Args>(args)...);
		size_ = new_size;
		return e;
	}

	template <class T>
	void Vector<T>::append_range(const Vector<T>& range)
	{
		//static_assert(std::is_base_of<coSpan<T>, A>::value, "_this should be an array");
		CP_ASSERT(size_ + range.Size() >= size_);
		const uint32 other_size = range.size_;
		const uint32 desired_size = size_ + other_size;
		reserve(desired_size);
		T* dest = data_ + size_;
		for (uint32 i = 0; i < other_size; ++i)
			new (dest + i) T(range[i]);
		size_ = desired_size;
	}

	template <class T>
	void Vector<T>::append_range(const std::span<T>& range)
	{
		//static_assert(std::is_base_of<coSpan<T>, A>::value, "_this should be an array");
		CP_ASSERT(size_ + range.size() >= size_);
		const uint32 other_size = (uint32)range.size();
		const uint32 desired_size = size_ + other_size;
		reserve(desired_size);
		T* dest = data_ + size_;
		for (uint32 i = 0; i < other_size; ++i)
			new (dest + i) T(range[i]);
		size_ = desired_size;
	}

	template <class T>
	void Vector<T>::pop_front(uint32 count)
	{
		CP_ASSERT(count <= size_);
		if (count)
		{
			const uint32 new_size = size_ - count;
			for (uint32 i = 0; i < size_; ++i)
				data_[i] = std::move(data_[i + count]);
			std::destroy(data_ + new_size, data_ + size_);
#ifdef CP_DEBUG
			mark_as_deleted(data_ + new_size, uint64(size_ - new_size) * sizeof(T));
#endif
			size_ = new_size;
		}
	}

	template <class T>
	void Vector<T>::shrink_to_fit()
	{
		set_capacity(size_);
	}

	template <class T> template <class U>
	void Vector<T>::erase_first(const U& value)
	{
		for (T& e : *this)
		{
			if (e == value)
			{
				erase(&e);
				break;
			}
		}
	}

	template <class T> template <class U>
	void Vector<T>::swap_and_pop_first(const U& value)
	{
		for (T& e : *this)
		{
			if (e == value)
			{
				swap_and_pop(&e);
				break;
			}
		}
	}

	template <class T> template <class U>
	void Vector<T>::fill(const U& value)
	{
		for (T& e : *this)
			e = value;
	}

	template <class T> template <class U>
	bool Vector<T>::contains(const U& value) const
	{
		for (const T& e : *this)
			if (e == value)
				return true;
		return false;
	}

	template <class T> template <class F>
	bool Vector<T>::contains_if(F func) const
	{
		for (const T& e : *this)
			if (func(e))
				return true;
		return false;
	}

	template <class T>
	T* Vector<T>::erase(T* it)
	{
		const T* endIt = data_ + size_;
		CP_ASSERT(it && it >= begin() && it < endIt);
		T* previous = it++;
		while (it != endIt)
		{
			*previous = std::move(*it);
			previous = it++;
		}
		std::destroy_at(previous);
#ifdef CP_DEBUG
		mark_as_deleted(previous, sizeof(T));
#endif
		--size_;
		return it;
	}

	template <class T>
	uint32 Vector<T>::erase_at(const uint32 index)
	{
		CP_ASSERT(index < size_);
		erase(&data_[index]);
		return index;
	}

	template <class T>
	uint32 Vector<T>::swap_and_pop_at(const uint32 index)
	{
		CP_ASSERT(index < size_);
		swap_and_pop(&data_[index]);
		return index;
	}

	template <class T>
	T* Vector<T>::swap_and_pop(T* it)
	{
		CP_ASSERT(it && it >= begin() && it < end());
		T* back = &data_[--size_];
		*it = std::move(*back);
		std::destroy_at(back);
#ifdef CP_DEBUG
		mark_as_deleted(back, sizeof(T));
#endif
		return it;
	}

	template <class T>
	void Vector<T>::operator=(const Vector<T>& other)
	{
		clear();
		reserve(other.size_);
		for (uint32 i = 0; i < other.size_; ++i)
			new (data_ + i) T(other[i]);
		size_ = other.size_;
	}

	template <class T>
	constexpr void Vector<T>::operator=(Vector<T>&& other)
	{
		swap(other);
	}

	template <class T>
	constexpr void Vector<T>::operator=(std::initializer_list<T> list)
	{
		clear();
		const uint32 list_size = (uint32)list.size();
		reserve(list_size);
		for (uint32 i = 0; i < list_size; ++i)
			new (data_ + i) T(list.begin()[i]);
		size_ = list_size;
	}

	template <class T>
	bool Vector<T>::operator==(const Vector<T>& other) const
	{
		if (size_ != other.size_)
			return false;
		for (uint32 i = 0; i < size_; ++i)
			if (data_[i] != other.data_[i])
				return false;
		return true;
	}

	template <class T>
	void Vector<T>::erase_before(const uint32 end_idx)
	{
		static_assert(std::is_trivially_copyable<T>::value, "Trivially copyable only");
		if (end_idx)
		{
			CP_ASSERT(end_idx <= size_);
			for (uint32 i = end_idx; i < size_; ++i)
				data_[i - end_idx] = std::move(data_[i]);
			size_ -= end_idx;
		}
	}
}
