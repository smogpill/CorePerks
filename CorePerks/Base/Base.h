// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "CorePerks/Generated/Config.h"
#include "CorePerks/Base/ExternalIncludes.h"
#include "CorePerks/Base/Platform.h"

// Base types
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint = unsigned int;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

namespace cp
{
	/// To avoid some gcc warnings with the comma operator
	inline void ReturnVoid(int) {}
}

#ifdef CP_MSVC
#define CP_BREAKPOINT() cp::ReturnVoid(IsDebuggerPresent() && (__debugbreak(), 1))
#else
#define CP_BREAKPOINT() std::breakpoint_if_debugging()
#endif

#define CP_SAFE_SCOPE(_x_) do { _x_ } while (!!false)

#include "CorePerks/Base/Diagnostics.h"
