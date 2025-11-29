// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/external_includes.h"

#ifdef CP_XXHASH
#ifdef CP_PRIVATE_INCLUDES
#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#include <xxHash/xxhash.h>
#endif
#endif

#ifdef CP_VMA
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#endif

#ifdef CP_TINYOBJLOADER
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#endif
