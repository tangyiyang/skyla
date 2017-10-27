/*
 * The Lua VM helper and binding functions.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_lua_conext_h
#define s2d_lua_conext_h

#include "s2d_common.h"

#ifdef S2D_ENABLE_LUA

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

NS_S2D

class context;
class lua_context {
public:
    static void stackDump (lua_State* L);
    static int call_lua(lua_State* L, int n, int r);

public:
    void init();
    void on_start(context* ctx, const char* script_path);
    void on_update(uint32_t dt);
    void on_destroy();

private:
    void register_lua_extensions();
    
public:
    lua_State* _lua_state;
    LUA_FUNC_HANDLER _event_handler;
};

NS_S2D_END

#endif /* end of S2D_ENABLE_LUA */

#endif
