/*
 * Lua bindings for Action.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef action_lua_binding_h
#define action_lua_binding_h

#include "lua_context.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int luaopen_skyla_action(lua_State* L);

NS_SKYLA_END

#endif

#endif
