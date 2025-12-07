// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class RefCounted
	{
	public:
		virtual ~RefCounted() = default;

		void add_ref() const { ++_ref_count; }
		void remove_ref() const;
		void disable_ref_counting() const { _ref_count = s_disabled_ref_counting; }

	protected:
		virtual void on_all_refs_removed() { delete this; }

	private:
		static constexpr uint32 s_disabled_ref_counting = 0xdddddddd;

		mutable std::atomic<uint32> _ref_count = 0;
	};

	template <class T>
	class RefPtr
	{
	public:
		RefPtr() = default;
		RefPtr(const RefPtr& other) : _ptr(other._ptr) { if (_ptr) _ptr->add_ref(); }
		RefPtr(RefPtr&& other) : _ptr(other._ptr) { other._ptr = nullptr; }
		explicit RefPtr(T* ptr) : _ptr(ptr) { if (_ptr) _ptr->add_ref(); }
		~RefPtr() { release(); }

		auto get() const -> T* { return _ptr; }
		void release() { if (_ptr) _ptr->remove_ref(); }

		auto operator=(const RefPtr& other) -> RefPtr&;
		auto operator=(RefPtr&& other) -> RefPtr&;
		auto operator=(T* ptr) -> RefPtr&;
		auto operator->() const -> T* { return _ptr; }
		auto operator*() const -> T& { CP_ASSERT(_ptr); return *_ptr; }
		bool operator==(const T* p) const { return _ptr == p; }
		bool operator!=(const T* p) const { return _ptr != p; }
		operator bool() const { return _ptr != nullptr; }

	private:
		T* _ptr = nullptr;
	};

	template <class T>
	class ConstRefPtr
	{
	public:
		~ConstRefPtr() { release(); }
		void release() { if (_ptr) _ptr->remove_ref(); }
	private:
		const T* _ptr = nullptr;
	};

	inline void RefCounted::remove_ref() const
	{
		if (--_ref_count == 0)
			const_cast<RefCounted*>(this)->on_all_refs_removed();
	}

	template <class T>
	auto RefPtr<T>::operator=(const RefPtr& other) -> RefPtr&
	{
		T* old_ptr = _ptr;
		_ptr = other._ptr;
		if (_ptr)
			_ptr->add_ref();
		if (old_ptr)
			old_ptr->remove_ref();
		return *this;
	}

	template <class T>
	auto RefPtr<T>::operator=(RefPtr&& other) -> RefPtr&
	{
		std::swap(_ptr, other._ptr);
		return *this;
	}

	template <class T>
	auto RefPtr<T>::operator=(T* ptr) -> RefPtr&
	{
		if (_ptr != ptr)
		{
			T* old_ptr = _ptr;
			_ptr = ptr;
			if (_ptr)
				_ptr->add_ref();
			if (old_ptr)
				old_ptr->remove_ref();
		}
		return *this;
	}
}
