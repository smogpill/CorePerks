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
		struct MoveIfAvailable
		{
		};

		template <class T>
		struct MoveIfAvailable<T, true>
		{
			static void move(void* from, void* to) { *static_cast<T*>(to) = std::move(*static_cast<T*>(from)); }
		};
		template <class T>
		struct MoveIfAvailable<T, false>
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
			static Type* get_type_static() { return T::get_type_static(); }
		};

		template <typename T>
		concept TypeBaseDefined = requires
		{
			typename T::Base; // Checks if T has a nested 'Base' type
			requires std::is_base_of_v<typename T::Base, T>;  // Checks if T::Base is actually its base
		};

		class CP_FORCE_SYMBOL_INCLUSION_ATTRIBUTE TypeAutoRegistrator
		{
		public:
			static TypeAutoRegistrator* get_registrator(int idx);
			static int get_registrator_count();
			static void add_registrator(TypeAutoRegistrator&);
		};

		template <class T>
		class CP_FORCE_SYMBOL_INCLUSION_ATTRIBUTE ClassTypeAutoRegistrator : public TypeAutoRegistrator
		{
		public:
			using InitFunc = void (*)(Type&);
			ClassTypeAutoRegistrator(InitFunc init_func)
			{
				Type* type = TypeStatic<T>::get_type_static();
				type->_init_type_func = init_func;
			}
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

		template <class T>
		void init_generics();
		auto create() -> void* { return _create_func(); }
		auto copy_create(const void* from) -> void* { return _copy_create_func(from); }
		template <class T>
		auto get() -> Type* { return detail::TypeStatic<T>::get_type(); }
		void set_init_type_func(auto func) { _init_type_func = func; }

	private:
		friend detail::ClassTypeAutoRegistrator;
		friend class TypeManager;

		void init(Type* type = nullptr);

		Type* _base = nullptr;
		uint32 _name_hash = 0;
		uint32 _size8 = 0;
		uint16 _alignment8 = 0;
		bool _initialized : 1 = false;
		bool _trivially_copyable : 1 = false;
		std::function<void* ()> _create_func;
		std::function<void* (const void*)> _copy_create_func;
		std::function<void(void*)> _construct_func;
		std::function<void(void*, const void*)> _copy_construct_func;
		std::function<void(void*)> _destruct_func;
		std::function<void(Type&)> _init_type_func;
		std::string _name;

		static std::vector<Type*> _types;
	};

	template <class T>
	void Type::init_generics()
	{
		using BaseType = TypeBase<T>::Type;
		_base = detail::TypeStatic<BaseType>::get_type_static();
		_size8 = sizeof(T);
		_alignment8 = alignof(T);
		_trivially_copyable = std::is_trivially_copyable<T>::value;
		using type_factory = detail::TypeFactory<T, !std::is_abstract_v<T>&& std::is_default_constructible_v<T>>;
		_create_func = &type_factory::create;
		_copy_create_func = &type_factory::copy_create;
		_construct_func = &type_factory::construct;
		_copy_construct_func = &type_factory::copy_construct;
		_destruct_func = &type_factory::destruct;
	}
}

#define _CP_TYPE_INITIALIZATION(_class_) \
	struct CP_FORCE_SYMBOL_INCLUSION_ATTRIBUTE _ClassInitializer_##_class_\
	{ \
		using Class = _class_; \
		using Base = TypeBase<Class>::Type; \
		using TypeClass = TypeClassHelper<Class>::Type; \
		_ClassInitializer_##_class_() \
		{ \
			Type* type = cp::detail::TypeStatic<_class_>::get_type_static(); \
			type->set_init_type_func(&init); \
		} \
		static void init(Type& type) \
		{ \
			type.init_generics<_class_>(); \
			user_init(static_cast<TypeClass*>(&type)); \
		} \
		static void user_init(TypeClass*); \
	} _class_initializer_##_class_; \
	void _ClassInitializer_##_class_::user_init(TypeClass* type)

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

#define CP_CLASS(_class_) \
	private: \
		using Self = _class_; \
		friend struct _ClassInitializer_##_class_; \
	public: \
		using TypeClass = TypeClassHelper<_class_>::Type; \
		static TypeClass* get_type_static(); \
	private:

#define CP_DEFINE_CLASS(_class_) \
	auto _class_::get_type_static() -> TypeClass* \
	{ \
		static TypeClass type(#_class_); \
		return &type; \
	} \
	_CP_TYPE_INITIALIZATION(_class_)

#define CP_DEFINE_TYPE(_type_) \
	_CP_TYPE_INITIALIZATION(_type_)

CP_TYPE(bool);
CP_TYPE(int8);
CP_TYPE(int16);
CP_TYPE(int32);
CP_TYPE(int64);
CP_TYPE(uint8);
CP_TYPE(uint16);
CP_TYPE(uint32);
CP_TYPE(uint64);
CP_TYPE(float);
CP_TYPE(double);
