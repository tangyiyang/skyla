//
//  Created by yiyang.tang on 2017/7/17.
//

#ifndef LUA_HANDLER_H
#define LUA_HANDLER_H

typedef int LUA_FUNC_HANDLER;

#ifdef __cplusplus
extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
    extern int luaopen_editor_core(lua_State* L);
    extern int luaopen_cjson(lua_State* L);
    extern int luaopen_lfs(lua_State* L);
}
#endif

#define EDITOR_INIT "EDITOR_INIT"
#define EDITOR_UPDATE "EDITOR_UPDATE"
#define EDITOR_DESTROY "EDITOR_DESTROY"

#include "level_editor_common.h"

NS_GAME_TOOL

class client;
class lua_handler {
public:
    lua_handler();
    ~lua_handler();

public:
    static void stackDump (lua_State *L);
    static int call_lua(lua_State *L, int n, int r);

public:
    void init();
    void update(uint32_t dt);
    void destroy();

private:
    void register_lua_extensions();
    
public:
    lua_State *_lua_state;
    LUA_FUNC_HANDLER _event_handler;
};

NS_GAME_TOOL_END
#endif
