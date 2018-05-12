/*
 * Lua bindings for Global Context.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_context_lua_binding_h
#define skyla_context_lua_binding_h

#include "lua_context.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int luaopen_skyla_context(lua_State* L);

NS_SKYLA_END

#endif

#endif
