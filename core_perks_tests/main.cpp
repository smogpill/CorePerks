// Vibeout (https://github.com/smogpill/vibeout)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
	cp::TypeManager type_manager;
	cp::Test::execute_all();
	return 0;
}
