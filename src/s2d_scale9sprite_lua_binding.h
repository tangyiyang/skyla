/*
 * Lua bindings for Scale9Sprite.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_scale9sprite_lua_binding_h
#define s2d_scale9sprite_lua_binding_h

#include "s2d_lua_context.h"

NS_S2D

#if (S2D_ENABLE_LUA == 1)

int luaopen_seal2d_scale9sprite(lua_State* L);

NS_S2D_END

#endif

#endif
