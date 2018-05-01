/*
 * Lua bindings for BMFont.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_bmfont_lua_binding_h
#define s2d_bmfont_lua_binding_h

#include "s2d_lua_context.h"

#if (S2D_ENABLE_LUA == 1)

NS_S2D

int luaopen_seal2d_bmfont(lua_State* L);

NS_S2D_END

#endif /* s2d_bmfont_lua_binding_h */

#endif
