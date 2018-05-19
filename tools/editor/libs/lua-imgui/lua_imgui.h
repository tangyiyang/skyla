#ifndef lua_imgui_h
#define lua_imgui_h

#include "imgui.h"

#ifdef __cplusplus
extern "C" {
#include "lauxlib.h"
}
#endif

int luaopen_imgui_core(lua_State* L);

#endif /* lua_imgui_h */
