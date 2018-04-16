/*
 * Lua bindings for Timer.
 * Author: yiyang
 * Created-at: 01/10/2018
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_timer_lua_binding_h
#define s2d_timer_lua_binding_h


#include "s2d_lua_context.h"

NS_S2D

int luaopen_seal2d_timer(lua_State* L);

NS_S2D_END

#endif /* s2d_timer_lua_binding_h */