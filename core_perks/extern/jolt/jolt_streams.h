// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/streams/binary_serializer.h"

#ifdef CP_JOLT
namespace cp
{
	BinaryInputStream& operator>>(BinaryInputStream& stream, JPH::Ref<JPH::Shape>& shape);
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const JPH::Ref<JPH::Shape>& shape);
}
#endif
