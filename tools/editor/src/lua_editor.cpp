#include "lua_editor.h"
#include "imgui.h"
#include "nfd.h"

int leditor_open_dialog(lua_State* L)
{
    const char* dir = luaL_checkstring(L, 1);
    nfdchar_t *out_path = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, (const nfdchar_t*)dir, &out_path);
    if (result == NFD_OKAY) {
        lua_pushstring(L, (const char*)out_path);
        return 1;
    } else if ( result == NFD_CANCEL ) {
        lua_pushstring(L, dir);
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
        lua_pushstring(L, dir);
        return 1;
    } else {
        luaL_error(L, "error pick folder: %s", NFD_GetError());
    }

    return 0;
}


int luaopen_editor_core(lua_State *L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "open_dialog", leditor_open_dialog},
        { "pick_folder", leditor_pick_folder},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}
