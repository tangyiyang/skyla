/*
 * Lua bindings for Node.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_node_lua_binding_h
#define skyla_node_lua_binding_h

#include "lua_context.h"

NS_SKYLA

#if (SKYLA_ENABLE_LUA == 1)

int luaopen_skyla_node(lua_State* L);

#endif

NS_SKYLA_END

#endif
