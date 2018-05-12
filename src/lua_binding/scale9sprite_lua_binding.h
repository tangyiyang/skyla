/*
 * Lua bindings for Scale9Sprite.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_scale9sprite_lua_binding_h
#define skyla_scale9sprite_lua_binding_h

#include "lua_context.h"

NS_SKYLA

#if (SKYLA_ENABLE_LUA == 1)

int luaopen_skyla_scale9sprite(lua_State* L);

#endif

NS_SKYLA_END

#endif
