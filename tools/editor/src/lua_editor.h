#ifndef lua_editor_h
#define lua_editor_h

#ifdef __cplusplus

extern "C" {
#include "lauxlib.h"
}
#endif

int luaopen_editor_core(lua_State *L);

#endif
