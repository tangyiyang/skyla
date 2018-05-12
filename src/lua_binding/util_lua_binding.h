#ifndef util_lua_binding_h
#define util_lua_binding_h

#include "lua_context.h"

NS_SKYLA

#if (SKYLA_ENABLE_LUA == 1)

int luaopen_skyla_util(lua_State* L);

#endif

NS_SKYLA_END

#endif
