/*
 * Lua bindings for Timer.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_timer_lua_binding_h
#define skyla_timer_lua_binding_h

#include "lua_context.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int luaopen_skyla_timer(lua_State* L);

NS_SKYLA_END

#endif

#endif
