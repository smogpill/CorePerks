// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/base/reflection/type_manager.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/containers/vector_extensions.h"
#include "core_perks/io/streams/binary_input_stream.h"
#include "core_perks/io/streams/binary_output_stream.h"

namespace cp
{
	Type::Type(const char* name)
		: _name(name)
		, _id(hash::fast::hash32(_name))
	{
		get_types().push_back(this);
		CP_ASSERT(get_id_to_type_map().find(_id) == get_id_to_type_map().end());
		get_id_to_type_map()[_id] = this;
	}

	Type::~Type()
	{
		get_id_to_type_map().erase(_id);
		swap_and_pop_first(get_types(), this);
	}

	void Type::init(Type* type)
	{
		if (type == nullptr)
		{
			if (_initialized)
				return;
			_initialized = true;
			type = this;
		}
		if (_base)
			_base->init(type);
		if (_init)
			_init(*type);
	}

	Type* Type::get_by_id(uint32 id)
	{
		const auto& map = get_id_to_type_map();
		auto it = map.find(id);
		if (it != map.end())
			return it->second;
		return nullptr;
	}

	bool Type::is_a(const Type& type) const
	{
		const Type* it = this;
		do
		{
			if (it == &type)
				return true;
			it = it->_base;
		} while (it);
		return false;
	}

	std::vector<Type*> Type::get_derived() const
	{
		std::vector<Type*> derived_types;
		for (Type* type : get_types())
			if (type != this && type->is_a(*this))
				derived_types.push_back(type);
		return derived_types;
	}

	std::vector<Type*>& Type::get_types()
	{
		static std::vector<Type*> types;
		return types;
	}

	std::unordered_map<uint32, Type*>& Type::get_id_to_type_map()
	{
		static std::unordered_map<uint32, Type*> map;
		return map;
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, Type*& type)
	{
		Type::Id id;
		stream >> id;
		type = Type::get_by_id(id);
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const Type* type)
	{
		stream << (type ? type->get_id() : Type::Id(0));
		return stream;
	}
}
