// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#include "core_perks/base/base.h"
#include "core_perks/memory/memory.h"
#include "core_perks/math/math.h"
#include "core_perks/containers/containers.h"
#ifdef CP_GLM
#include "core_perks/glm/glm.h"
#endif
#include "core_perks/io/io.h"
#include "core_perks/patterns/patterns.h"
#ifdef CP_SDL
#include "core_perks/sdl/sdl.h"
#endif
#include "core_perks/threading/threading.h"
#ifdef CP_VULKAN
#include "core_perks/vulkan/vulkan.h"
#endif
#ifdef CP_TRACY
#define TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif
