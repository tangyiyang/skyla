#include "lua_handler.h"

int leditor_inject(lua_State* L)
{
    if(!lua_istable(L, -1)) {
        printf("seal.start require a table to start.\n");
        exit(1);
    }

    lua_getfield(L, -1, "on_init");
    lua_setfield(L, LUA_REGISTRYINDEX, EDITOR_INIT);

    lua_getfield(L, -1, "on_update");
    lua_setfield(L, LUA_REGISTRYINDEX, EDITOR_UPDATE);

    lua_getfield(L, -1, "on_destory");
    lua_setfield(L, LUA_REGISTRYINDEX, EDITOR_DESTROY);


    lua_pop(L, -1);
    return 0;
}

#ifdef __cplusplus

extern "C" {

int luaopen_editor_core(lua_State *L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "inject", leditor_inject},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

}
#endif
