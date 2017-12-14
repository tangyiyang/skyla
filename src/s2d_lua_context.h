/*
 * The Lua VM helper and binding functions.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_lua_conext_h
#define s2d_lua_conext_h

#include "s2d_common.h"

#if (S2D_ENABLE_LUA == 1)

typedef int LUA_FUNC_HANDLER;

#ifdef __cplusplus
extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
}
#endif

#define CONTEXT_START "CONTEXT_START"
#define CONTEXT_UPDATE "CONTEXT_UPDATE"
#define CONTEXT_DESTROY "CONTEXT_DESTROY"

#define SEAL2D_EVENT_TOUCH "seal2d_event_touch"
#define SEAL2D_USER_FUNC_TABLE "seal2d_user_funcs"
#define TRACE_BACK_FUNC_INDEX (1)

NS_S2D

class context;
class lua_context {
public:
    static void stackDump (lua_State* L);
    static int call_lua(lua_State* L, int n, int r);

public:
    void init();
    void on_start(context* ctx, const char* script_path);
    void on_update(float dt);
    void on_destroy();

private:
    void register_lua_extensions(lua_State* L);
    
public:
    lua_State* _lua_state;
    LUA_FUNC_HANDLER _event_handler;
};

NS_S2D_END

#endif /* end of S2D_ENABLE_LUA */

#endif
