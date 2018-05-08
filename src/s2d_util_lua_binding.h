#ifndef s2d_util_lua_binding_h
#define s2d_util_lua_binding_h

#include "s2d_lua_context.h"

NS_S2D

#if (S2D_ENABLE_LUA == 1)

int luaopen_seal2d_util(lua_State* L);

#endif

NS_S2D_END

#endif
