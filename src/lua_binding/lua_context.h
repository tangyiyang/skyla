/*
 * The Lua VM helper and binding functions.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_lua_conext_h
#define skyla_lua_conext_h

#include "skyla_common.h"

#if (SKYLA_ENABLE_LUA == 1)

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

#define SKYLA_EVENT_TOUCH "skyla_event_touch"
#define SKYLA_TOUCH_FUNC_TABLE  "skyla_touch_funcs"
#define SKYLA_UPDATE_FUNC_TABLE "skyla_update_funcs"
#define SKYLA_USER_TIMER_TABLE "skyla_user_timers"

#define register_user_table(name) do { \
            lua_newtable(L); \
            lua_setfield(L, LUA_REGISTRYINDEX, name); \
        }while(0); \


#define STACK_DUMP(L) lua_context::dump_stack(L);
NS_SKYLA

class context;
class lua_context {
public:
    static void dump_stack(lua_State* L);
    static int call_lua(lua_State* L, int n, int r);

public:
    bool load_lua_file(const char* script_path);

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

NS_SKYLA_END

#endif

#endif
