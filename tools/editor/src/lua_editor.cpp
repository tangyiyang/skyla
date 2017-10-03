#include "lua_handler.h"

#include "nfd.h"

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

int leditor_open_dialog(lua_State* L)
{
    const char* dir = luaL_checkstring(L, 1);
    nfdchar_t *out_path = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, (const nfdchar_t*)dir, &out_path);
    if (result == NFD_OKAY) {
        lua_pushstring(L, (const char*)out_path);
        return 1;
    } else if ( result == NFD_CANCEL ) {
        lua_pushstring(L, "");
        return 1;
    } else {
        luaL_error(L, "error open dialog: %s", NFD_GetError());
    }

    return 0;
}

int leditor_pick_folder(lua_State* L)
{
    const char* dir = luaL_checkstring(L, 1);
    nfdchar_t *out_path = NULL;
    nfdresult_t result = NFD_PickFolder((const nfdchar_t*)dir, &out_path);
    if (result == NFD_OKAY) {
        lua_pushstring(L, (const char*)out_path);
        return 1;
    } else if ( result == NFD_CANCEL ) {
        lua_pushstring(L, "");
        return 1;
    } else {
        luaL_error(L, "error pick folder: %s", NFD_GetError());
    }

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
        { "open_dialog", leditor_open_dialog},
        { "pick_folder", leditor_pick_folder},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

}
#endif
