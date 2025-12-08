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
		explicit Vector(AllocTag tag) : _tag(tag) {}
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
		auto emplace_back(Args&&... args) -> T&;
		void append_range(const Vector<T>& range);
		void append_range(const std::span<T>& range);
		auto pop_back() -> const T& { CP_ASSERT(_size); return _data[--_size]; }
		void pop_front(uint32 count);
		auto begin() -> T* { return _data; }
		auto begin() const -> const T* { return _data; }
		auto cbegin() const -> const T* { return _data; }
		auto end() -> T* { return _data + _size; }
		auto end() const -> const T* { return _data + _size; }
		auto cend() const -> const T* { return _data + _size; }
		auto data() -> T* { return _data; }
		auto data() const -> const T* { return _data; }
		auto back() -> T& { CP_ASSERT(_size); return _data[_size - 1]; }
		auto back() const -> const T& { CP_ASSERT(_size); return _data[_size - 1]; }
		auto operator[] (uint32 i) -> T& { CP_ASSERT(i < _size); return _data[i]; }
		auto operator[] (uint32 i) const -> const T& { CP_ASSERT(i < _size); return _data[i]; }
		void operator=(const Vector<T>& other);
		constexpr void operator=(Vector<T>&& other);
		constexpr void operator=(std::initializer_list<T> list);
		bool operator==(const Vector<T>& other) const;
		bool operator!=(const Vector<T>& other) const { return !operator==(other); }
		bool empty() const { return _size == 0; }
		auto size() const -> uint32 { return _size; }
		auto capacity() const -> uint32 { return _capacity; }
		void swap(Vector<T>& other) { swap_memory(*this, other); }
		void shrink_to_fit();
		auto erase(T* it) -> T*;
		auto erase(const T* it) -> const T* { return (const T*)erase((T*)it); }
		template <class U>
		void erase_first(const U& value);
		auto erase_at(uint32 index) -> uint32;
		void erase_before(uint32 end_idx);
		auto swap_and_pop(T* it) -> T*;
		auto swap_and_pop(const T* it) -> const T* { return (const T*)swap_and_pop((T*)it); }
		template <class U>
		void swap_and_pop_first(const U& value);
		auto swap_and_pop_at(uint32 index) -> uint32;
		template <class U>
		bool contains(const U& value) const;
		template <class F>
		bool contains_if(F func) const;
		template <class U>
		void fill(const U& value);

	private:
		T* _data = nullptr;
		uint32 _size = 0;
		uint32 _capacity = 0;
		AllocTag _tag = 0;
	};


	uint32 compute_best_Vector_capacity(uint32 desired_capacity);

	template <class T>
	Vector<T>::Vector(uint32 initial_size, AllocTag tag)
		: _tag(tag)
	{
		resize(initial_size);
	}

	template <class T>
	Vector<T>::Vector(uint32 initial_size, const T& value, AllocTag tag)
		: _tag(tag)
	{
		resize(initial_size, value);
	}

	template <class T>
	Vector<T>::Vector(const Vector<T>& other, AllocTag tag)
		: _tag(tag)
	{
		operator=(other);
	}

	template <class T>
	Vector<T>::Vector(Vector<T>&& other)
		: _data(other._data)
		, _size(other._size)
		, _capacity(other._capacity)
		: _tag(tag)
	{
		other._data = nullptr;
		other._size = 0;
		other._capacity = 0;
	}

	template <class T>
	Vector<T>::Vector(Vector<T>&& other, AllocTag tag)
		: _tag(tag)
	{
		if (_tag == other._tag)
			operator=(std::move(other));
		else
			operator=(other);
	}

	template <class T>
	Vector<T>::Vector(std::initializer_list<T> list, AllocTag tag)
		: _tag(tag)
	{
		operator=(list);
	}

	template <class T>
	Vector<T>::~Vector()
	{
		if (_data)
		{
			std::destroy(_data, _data + _size);
			CP_ASSERT(_allocator);
			free(_data);
		}
	}

	template <class T>
	void Vector<T>::clear()
	{
		if (!_size)
			return;
		std::destroy(_data, _data + _size);
#ifdef CP_DEBUG
		fill_as_deleted(_data, uint64(_size) * sizeof(T));
#endif
		_size = 0;
	}

	template <class T>
	void Vector<T>::delete_and_clear()
	{
		for (T p : *this)
			delete p;
		clear();
	}

	template<class T>
	inline void Vector<T>::reverse_delete_and_clear()
	{
		for (uint32 i = _size; i--;)
			delete _data[i];
		clear();
	}

	template <class T>
	void Vector<T>::set_capacity(uint32 new_capacity)
	{
		CP_ASSERT(new_capacity >= _size);
		if (new_capacity != _capacity)
		{
			// Aligned to min 16 bytes:
			// - For simplicity. For example for working using SIMD on a float array.
			// - Seems faster on Intel architectures https://software.intel.com/en-us/articles/data-alignment-when-migrating-to-64-bit-intel-architecture).
			const uint alignment = alignof(T) > 16 ? alignof(T) : 16;
			CP_ASSERT(_allocator);
			T* new_buffer;
			const size_t new_capacity8 = uint64(new_capacity) * sizeof(T);
			new_buffer = static_cast<T*>(aligned_alloc(alignment, new_capacity8, _tag));
			if (_data)
			{
				std::uninitialized_move(_data, _data + _size, new_buffer);
				std::destroy(_data, _data + _size);
				free(_data);
			}
			_data = new_buffer;
			_capacity = new_capacity;
		}
	}

	template <class T>
	CP_FORCE_INLINE void Vector<T>::reserve(uint32 size)
	{
		if (size > _capacity)
			set_capacity(compute_best_Vector_capacity(size));
	}

	template <class T>
	void Vector<T>::resize(uint32 new_size)
	{
		if (new_size != _size)
		{
			if (new_size > _size)
			{
				reserve(new_size);
				::new (_data + _size) T[new_size - _size];
			}
			else
			{
				std::destroy(_data + new_size, _data + _size);
#ifdef CP_DEBUG
				fill_as_deleted(_data + new_size, uint64(_size - new_size) * sizeof(T));
#endif
			}
			_size = new_size;
		}
	}

	template <class T>
	void Vector<T>::resize(uint32 new_size, const T& value)
	{
		if (new_size != _size)
		{
			if (new_size > _size)
			{
				reserve(new_size);
				for (uint32 i = _size; i < new_size; ++i)
					::new (_data + i) T(value);
			}
			else
			{
				std::destroy(_data + new_size, _data + _size);
#ifdef CP_DEBUG
				fill_as_deleted(_data + new_size, uint64(_size - new_size) * sizeof(T));
#endif
			}
			_size = new_size;
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
		const uint32 new_size = _size + 1;
		reserve(new_size);
		T& e = _data[_size];
		new (&e) T(std::forward<Args>(args)...);
		_size = new_size;
		return e;
	}

	template <class T>
	void Vector<T>::append_range(const Vector<T>& range)
	{
		//static_assert(std::is_base_of<coSpan<T>, A>::value, "_this should be an array");
		CP_ASSERT(_size + range.Size() >= _size);
		const uint32 other_size = range._size;
		const uint32 desired_size = _size + other_size;
		reserve(desired_size);
		T* dest = _data + _size;
		for (uint32 i = 0; i < other_size; ++i)
			new (dest + i) T(range[i]);
		_size = desired_size;
	}

	template <class T>
	void Vector<T>::append_range(const std::span<T>& range)
	{
		//static_assert(std::is_base_of<coSpan<T>, A>::value, "_this should be an array");
		CP_ASSERT(_size + range.size() >= _size);
		const uint32 other_size = (uint32)range.size();
		const uint32 desired_size = _size + other_size;
		reserve(desired_size);
		T* dest = _data + _size;
		for (uint32 i = 0; i < other_size; ++i)
			new (dest + i) T(range[i]);
		_size = desired_size;
	}

	template <class T>
	void Vector<T>::pop_front(uint32 count)
	{
		CP_ASSERT(count <= _size);
		if (count)
		{
			const uint32 new_size = _size - count;
			for (uint32 i = 0; i < _size; ++i)
				_data[i] = std::move(_data[i + count]);
			std::destroy(_data + new_size, _data + _size);
#ifdef CP_DEBUG
			fill_as_deleted(_data + new_size, uint64(_size - new_size) * sizeof(T));
#endif
			_size = new_size;
		}
	}

	template <class T>
	void Vector<T>::shrink_to_fit()
	{
		set_capacity(_size);
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
	void Vector<T>::unordered_erase_first(const U& value)
	{
		for (T& e : *this)
		{
			if (e == value)
			{
				unordered_erase(&e);
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
		const T* endIt = _data + _size;
		CP_ASSERT(it && it >= begin() && it < endIt);
		T* previous = it++;
		while (it != endIt)
		{
			*previous = std::move(*it);
			previous = it++;
		}
		std::destroy_at(previous);
#ifdef CP_DEBUG
		fill_as_deleted(previous, sizeof(T));
#endif
		--_size;
		return it;
	}

	template <class T>
	uint32 Vector<T>::erase_at(const uint32 index)
	{
		CP_ASSERT(index < _size);
		erase(&_data[index]);
		return index;
	}

	template <class T>
	uint32 Vector<T>::swap_and_pop_at(const uint32 index)
	{
		CP_ASSERT(index < _size);
		swap_and_pop(&_data[index]);
		return index;
	}

	template <class T>
	T* Vector<T>::swap_and_pop(T* it)
	{
		CP_ASSERT(it && it >= begin() && it < end());
		T* back = &_data[--_size];
		*it = std::move(*back);
		std::destroy_at(back);
#ifdef CP_DEBUG
		fill_as_deleted(back, sizeof(T));
#endif
		return it;
	}

	template <class T>
	void Vector<T>::operator=(const Vector<T>& other)
	{
		clear();
		reserve(other._size);
		for (uint32 i = 0; i < other._size; ++i)
			new (_data + i) T(other[i]);
		_size = other._size;
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
			new (_data + i) T(list.begin()[i]);
		_size = list_size;
	}

	template <class T>
	bool Vector<T>::operator==(const Vector<T>& other) const
	{
		if (_size != other._size)
			return false;
		for (uint32 i = 0; i < _size; ++i)
			if (_data[i] != other._data[i])
				return false;
		return true;
	}

	template <class T>
	void Vector<T>::erase_before(const uint32 end_idx)
	{
		static_assert(std::is_trivially_copyable<T>::value, "Trivially copyable only");
		if (end_idx)
		{
			CP_ASSERT(end_idx <= _size);
			for (uint32 i = end_idx; i < _size; ++i)
				_data[i - end_idx] = std::move(_data[i]);
			_size -= end_idx;
		}
	}
}
