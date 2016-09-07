////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_script_space.h
//	Created 	: 22.09.2003
//  Modified 	: 22.09.2003
//	Author		: Dmitriy Iassenev
//	Description : XRay Script space
////////////////////////////////////////////////////////////////////////////

#pragma once

// Lua
#pragma warning(push)
#pragma warning(disable:4244 4267 4297 4530 4913 4995)

//#define LUABIND_NO_EXCEPTIONS

extern "C" {
	//#define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
//	#include <lua/luajit.h>
}

// Lua-bind
#include <luabind/luabind.hpp>
#include <luabind/object.hpp>

#pragma warning(pop)

typedef lua_State CLuaVirtualMachine;

struct SMemberCallback {
	luabind::functor<void>	*m_lua_function;
	luabind::object			*m_lua_object;
	shared_str				m_method_name;
};

#include "ai_script_lua_space.h"
