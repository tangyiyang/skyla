/*
 * Lua bindings for Primitive.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_primitive_node_lua_binding_h
#define skyla_primitive_node_lua_binding_h

#include "lua_context.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int luaopen_skyla_primitive(lua_State* L);

NS_SKYLA_END

#endif

#endif

