// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class Type;

	namespace detail
	{
		template <class T, bool>
		struct TypeMove
		{
		};

		template <class T>
		struct TypeMove<T, true>
		{
			static void move(void* from, void* to) { *static_cast<T*>(to) = std::move(*static_cast<T*>(from)); }
		};
		template <class T>
		struct TypeMove<T, false>
		{
			static void move(void* from, void* to) { CP_ASSERT(false); }
		};

		template <class T, bool>
		struct TypeFactory
		{
		};

		template <class T>
		struct TypeFactory<T, true>
		{
			static void* create() { return new T(); }
			static void* copy_create(const void* from) { return new T(*static_cast<const T*>(from)); }
			static void construct(void* p) { CP_ASSERT(p); new (p) T(); }
			static void copy_construct(void* p, const void* from) { CP_ASSERT(p); new (p) T(*static_cast<const T*>(from)); }
			static void destruct(void* p) { CP_ASSERT(p); static_cast<T*>(p)->~T(); }
		};

		template <class T>
		struct TypeFactory<T, false>
		{
			static void* create() { CP_ASSERT(false); return nullptr; }
			static void* copy_create(const void*) { CP_ASSERT(false); return nullptr; }
			static void construct(void* p) { CP_ASSERT(false); }
			static void copy_construct(void* p, const void* from) { CP_ASSERT(false); }
			static void destruct(void* p) { CP_ASSERT(false); }
		};

		template <class T, class = void>
		struct TypeStatic
		{
			static Type* get_type_static() { return nullptr; }
		};

		template <class T>
		struct TypeStatic<T, std::void_t<decltype(T::get_type_static)>>
		{
			static Type* get_type_static() { return &T::get_type_static(); }
		};

		template <typename T>
		concept TypeBaseDefined = requires
		{
			typename T::Base; // Checks if T has a nested 'Base' type
			requires std::is_base_of_v<typename T::Base, T>;  // Checks if T::Base is actually its base
		};
	}

	template <typename T, typename = void>
	struct TypeClassHelper
	{
		using Type = Type;
	};

	template <typename T>
	struct TypeClassHelper<T, std::void_t<typename T::UserTypeClass>>
	{
		using Type = typename T::UserTypeClass;
	};

	template <typename T, typename = void>
	struct TypeBase
	{
		using Type = void;
	};

	template <typename T>
	struct TypeBase<T, std::void_t<typename T::BasePublic>>
	{
		using Type = typename T::BasePublic;
	};

	class Type
	{
	public:
		Type(const char* name);
		virtual ~Type() = default;

		template <class T = void>
		auto create() const -> T* { return static_cast<T*>(_create()); }
		auto copy_create(const void* from) const -> void* { return _copy_create(from); }
		void construct(void* ptr) const { _construct(ptr); }
		void copy_construct(void* ptr, const void* from) const { _copy_construct(ptr, from); }
		void destruct(void* ptr) const { _destruct(ptr); }
		template <class T>
		static auto get() -> Type* { return detail::TypeStatic<T>::get_type_static(); }
		bool is_a(const Type& type) const;
		std::vector<Type*> get_derived() const;

		template <class T>
		void _init_generics();
		void _set_init_func(auto func) { _init = func; }

	private:
		friend class TypeManager;

		void init(Type* type = nullptr);

		Type* _base = nullptr;
		uint32 _name_hash = 0;
		uint32 _size8 = 0;
		uint16 _alignment8 = 0;
		bool _initialized : 1 = false;
		bool _trivially_copyable : 1 = false;
		std::function<void*()> _create;
		std::function<void*(const void*)> _copy_create;
		std::function<void(void*)> _construct;
		std::function<void(void*, const void*)> _copy_construct;
		std::function<void(void*, void*)> _move;
		std::function<void(void*)> _destruct;
		std::function<void(Type&)> _init;
		std::string _name;

		static std::vector<Type*> _types;
	};

	template <class T>
	void Type::_init_generics()
	{
		using BaseType = TypeBase<T>::Type;
		_base = detail::TypeStatic<BaseType>::get_type_static();
		_size8 = sizeof(T);
		_alignment8 = alignof(T);
		_trivially_copyable = std::is_trivially_copyable<T>::value;
		using type_factory = detail::TypeFactory<T, !std::is_abstract_v<T> && std::is_default_constructible_v<T>>;
		_create = &type_factory::create;
		_copy_create = &type_factory::copy_create;
		_construct = &type_factory::construct;
		_copy_construct = &type_factory::copy_construct;
		_move = &detail::TypeMove<T, std::is_copy_assignable<T>::value>::move;
		_destruct = &type_factory::destruct;
	}
}

#define _CP_TYPE_INITIALIZATION(_type_) \
	struct CP_FORCE_SYMBOL_INCLUSION_ATTRIBUTE _TypeInitializer_##_type_\
	{ \
		using Self = _type_; \
		using Base = cp::TypeBase<_type_>::Type; \
		using TypeClass = cp::TypeClassHelper<_type_>::Type; \
		_TypeInitializer_##_type_() \
		{ \
			cp::Type* type = cp::detail::TypeStatic<_type_>::get_type_static(); \
			type->_set_init_func(&init); \
		} \
		static void init(cp::Type& type) \
		{ \
			type._init_generics<_type_>(); \
			user_init(static_cast<TypeClass*>(&type)); \
		} \
		static void user_init(TypeClass*); \
	} _type_initializer_##_type_; \
	void _TypeInitializer_##_type_::user_init(TypeClass* type)

#define _CP_CLASS_SHARED(_class_) \
	private: \
		using Self = _class_; \
		friend struct _TypeInitializer_##_class_; \
	public: \
		using TypeClass = TypeClassHelper<_class_>::Type; \
		static TypeClass& get_type_static(); \
	private:

#define CP_BASE(_class_) \
	private:\
		using Base = _class_; \
	public:\
		using BasePublic = Base; \
	private:

#define CP_TYPE_CLASS(_type_class_) \
	public:\
		using UserTypeClass = _type_class_;\
	private:

#define CP_TYPE(_type_) \
	template <> \
	struct cp::detail::TypeStatic<_type_> \
	{ \
		static Type* get_type_static() \
		{ \
			static Type type(#_type_); \
			return &type; \
		} \
	} 

#define CP_CLASS_NO_POLYMORPHISM(_class_) \
	_CP_CLASS_SHARED(_class_) \
	public:\
		TypeClass& get_type() const { return get_type_static(); } \
	private:

#define CP_CLASS(_class_) \
	_CP_CLASS_SHARED(_class_) \
	public:\
		virtual TypeClass& get_type() const { return get_type_static(); } \
	private:

#define CP_DEFINE_CLASS(_class_) \
	auto _class_::get_type_static() -> TypeClass& \
	{ \
		static TypeClass type(#_class_); \
		return type; \
	} \
	_CP_TYPE_INITIALIZATION(_class_)

#define CP_DEFINE_TYPE(_type_) \
	_CP_TYPE_INITIALIZATION(_type_)

