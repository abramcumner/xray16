#include "stdafx.h"

// Порядок следования глобальных переменных важен! Не менять!

#ifndef PURE_ALLOC
//#	ifndef USE_MEMORY_MONITOR
#		define USE_DL_ALLOCATOR
//#	endif // USE_MEMORY_MONITOR
#endif // PURE_ALLOC

#ifndef USE_DL_ALLOCATOR
static void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	(void)ud;
	(void)osize;
	if (nsize == 0) {
		xr_free(ptr);
		return	NULL;
	}
	else
#ifdef DEBUG_MEMORY_NAME
		return Memory.mem_realloc(ptr, nsize, "LUA");
#else // DEBUG_MEMORY_MANAGER
		return Memory.mem_realloc(ptr, nsize);
#endif // DEBUG_MEMORY_MANAGER
}
#else // USE_DL_ALLOCATOR

#include "../../xrCore/memory_allocator_options.h"

#ifdef USE_ARENA_ALLOCATOR
static const u32	s_arena_size = 8 * 1024 * 1024;
static char			s_fake_array[s_arena_size];
doug_lea_allocator	g_render_lua_allocator(s_fake_array, s_arena_size, "render:lua");
#else // #ifdef USE_ARENA_ALLOCATOR
doug_lea_allocator	g_render_lua_allocator(0, 0, "render:lua");
#endif // #ifdef USE_ARENA_ALLOCATOR

#endif // USE_DL_ALLOCATOR

CRender RImplementation;