// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#if defined(CP_DEBUG) || defined(CP_DEV)
#define CP_DEBUG_OR_DEV
#endif
#if defined(CP_PROFILE) || defined(CP_RELEASE)
#define CP_PROFILE_OR_RELEASE
#endif

/*
#ifdef CP_DEBUG
#define CP_DEBUG_EXPRESSION(_expr_) _expr_
#else
#define CP_DEBUG_EXPRESSION(_expr_)
#endif
*/

#include "core_perks/generated/config.h"
#include "core_perks/extern/public_externals.h"
#include "core_perks/base/platform.h"

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
using uintptr = uintptr_t;
//using ndbool = [[nodiscard]] bool;

namespace cp
{
	/// To avoid some gcc warnings with the comma operator
	constexpr void return_void(int) {}
	constexpr bool return_false() { return false; }
	constexpr bool return_true() { return true; }
}

#define _CP_STRINGIFY(_x_) #_x_
#define CP_STRINGIFY(_x_) _CP_STRINGIFY(_x_)
#define _CP_CONCAT(_x_, _y_) _x_ ## _y_
#define CP_CONCAT(_x_, _y_) _CP_CONCAT(_x_, _y_)
#define CP_PRAGMA_MESSAGE(_x_)	_Pragma(message(__FILE__ "("  CP_STRINGIFY(__LINE__) "): " _x_))

#ifdef CP_MSVC
	#define CP_BREAKPOINT() cp::return_void(IsDebuggerPresent() && (__debugbreak(), 1))
	#define CP_FORCE_INLINE __forceinline
	#define CP_FORCE_SYMBOL_INCLUSION_ATTRIBUTE __declspec(dllexport)
#else
	#define CP_BREAKPOINT() std::breakpoint_if_debugging()
	#define CP_FORCE_INLINE [[gnu::always_inline]]
	#define CP_FORCE_SYMBOL_INCLUSION_ATTRIBUTE __attribute__ ((used))
#endif

#ifdef CP_DEBUG_OR_DEV
#	ifdef CP_MSVC
#		define CP_DEOPTIMIZE_PUSH() __pragma(optimize("", push)) __pragma(optimize("", off)) CP_PRAGMA_MESSAGE("Deoptimize push")
#		define CP_DEOPTIMIZE_POP() __pragma(optimize("", pop)) CP_PRAGMA_MESSAGE("Deoptimize pop")
#	else
#		define CP_DEOPTIMIZE_PUSH() _Pragma("GCC push_options") _Pragma("GCC optimize (\"O0\")") CP_PRAGMA_MESSAGE("Deoptimize push")
#		define CP_DEOPTIMIZE_POP()  _Pragma("GCC pop_options")  CP_PRAGMA_MESSAGE("Deoptimize pop")
#	endif
#else
	#define CP_DEOPTIMIZE_PUSH()
	#define CP_DEOPTIMIZE_POP()
#endif

#define CP_SAFE_SCOPE(_x_) do { _x_ } while (0)

#define CP_AS_CONST(_member_) const_cast<std::remove_reference_t<decltype(*this)>*>(this)->_member_

#include "core_perks/base/diagnostics.h"
#include "core_perks/base/reflection/reflection.h"

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
