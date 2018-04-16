/*
 * Lua bindings for Global Context.
 * Author: yiyang
 * Created-at: 01/10/2018
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_context_lua_binding_h
#define s2d_context_lua_binding_h

#include "s2d_lua_context.h"

#if (S2D_ENABLE_LUA == 1)

NS_S2D

int luaopen_seal2d_context(lua_State* L);

NS_S2D_END

#endif

#endif
