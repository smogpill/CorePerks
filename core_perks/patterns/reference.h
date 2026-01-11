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

		void add_ref() const { ++ref_count_; }
		void remove_ref() const;
		void disable_ref_counting() const { ref_count_ = disabled_ref_counting; }

	protected:
		virtual void on_all_refs_removed() { delete this; }

	private:
		static constexpr uint32 disabled_ref_counting = 0xdddddddd;

		mutable std::atomic<uint32> ref_count_ = 0;
	};

	template <class T>
	class RefPtr
	{
	public:
		RefPtr() = default;
		RefPtr(const RefPtr& other) : ptr_(other.ptr_) { if (ptr_) add_ref(); }
		template <class U>
		RefPtr(const RefPtr<U>& other) : ptr_(static_cast<T*>(other.ptr_)) { if (ptr_) add_ref(); }
		RefPtr(RefPtr&& other) : ptr_(other.ptr_) { other.ptr_ = nullptr; }
		template <class U>
		RefPtr(RefPtr<U>&& other) : ptr_(static_cast<T*>(other.ptr_)) { other.ptr_ = nullptr; }
		explicit RefPtr(T* ptr) : ptr_(ptr) { if (ptr_) add_ref(); }
		template <class U>
		explicit RefPtr(U* ptr) : ptr_(static_cast<T*>(ptr)) { if (ptr_) add_ref(); }
		~RefPtr() { reset(); }

		T* get() const { return ptr_; }
		void reset() { if (ptr_) remove_ref(); }

		RefPtr& operator=(const RefPtr& other);
		RefPtr& operator=(RefPtr&& other);
		RefPtr& operator=(T* ptr);
		T* operator->() const { return ptr_; }
		T& operator*() const { CP_ASSERT(ptr_); return *ptr_; }
		bool operator==(const T* p) const { return ptr_ == p; }
		bool operator!=(const T* p) const { return ptr_ != p; }
		operator bool() const { return ptr_ != nullptr; }

	private:
		template <class U> friend class RefPtr;

		void add_ref() { ((RefCounted*)ptr_)->add_ref(); }
		void remove_ref() { ((RefCounted*)ptr_)->remove_ref(); }

		T* ptr_ = nullptr;
	};

	/*

	template <class T>
	class ConstRefPtr
	{
	public:
		ConstRefPtr() = default;
		ConstRefPtr(const RefPtr& other) : ptr_(other.ptr_) { if (ptr_) ((RefCounted*)ptr_)->add_ref(); }
		template <class U>
		ConstRefPtr(const ConstRefPtr<U>& other) : ptr_(static_cast<T*>(other.ptr_)) { if (ptr_) ((RefCounted*)ptr_)->add_ref(); }
		ConstRefPtr(ConstRefPtr&& other) : ptr_(other.ptr_) { other.ptr_ = nullptr; }
		template <class U>
		ConstRefPtr(ConstRefPtr<U>&& other) : ptr_(static_cast<T*>(other.ptr_)) { other.ptr_ = nullptr; }
		explicit ConstRefPtr(T* ptr) : ptr_(ptr) { if (ptr_) ptr_->add_ref(); }
		template <class U>
		explicit ConstRefPtr(U* ptr) : ptr_(static_cast<T*>(ptr)) { if (ptr_) ptr_->add_ref(); }
		~ConstRefPtr() { reset(); }

		T* get() const { return ptr_; }
		void reset() { if (ptr_) ((RefCounted*)ptr_)->remove_ref(); }

		ConstRefPtr& operator=(const ConstRefPtr& other);
		ConstRefPtr& operator=(ConstRefPtr&& other);
		ConstRefPtr& operator=(T* ptr);
		T* operator->() const { return ptr_; }
		T& operator*() const { CP_ASSERT(ptr_); return *ptr_; }
		bool operator==(const T* p) const { return ptr_ == p; }
		bool operator!=(const T* p) const { return ptr_ != p; }
		operator bool() const { return ptr_ != nullptr; }
	private:
		template <class U> friend class ConstRefPtr;
		const T* ptr_ = nullptr;
	};
	*/

	inline void RefCounted::remove_ref() const
	{
		if (--ref_count_ == 0)
			const_cast<RefCounted*>(this)->on_all_refs_removed();
	}

	template <class T>
	RefPtr<T>& RefPtr<T>::operator=(const RefPtr& other)
	{
		T* old_ptr = ptr_;
		ptr_ = other.ptr_;
		if (ptr_)
			ptr_->add_ref();
		if (old_ptr)
			old_ptr->remove_ref();
		return *this;
	}

	template <class T>
	RefPtr<T>& RefPtr<T>::operator=(RefPtr&& other)
	{
		std::swap(ptr_, other.ptr_);
		return *this;
	}

	template <class T>
	RefPtr<T>& RefPtr<T>::operator=(T* ptr)
	{
		if (ptr_ != ptr)
		{
			T* old_ptr = ptr_;
			ptr_ = ptr;
			if (ptr_)
				ptr_->add_ref();
			if (old_ptr)
				old_ptr->remove_ref();
		}
		return *this;
	}
}
