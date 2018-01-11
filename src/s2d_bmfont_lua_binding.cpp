#include "s2d_bmfont_lua_binding.h"
#include "s2d_bmfont.h"

NS_S2D

int lseal2d_new_bmfont(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        const char* text = luaL_checkstring(L, 1);
        const char* font_atlas_path = luaL_checkstring(L, 2);

        bmfont* n = new bmfont();
        n->init(text, font_atlas_path);

        lua_pushlightuserdata(L, n);
        return 1;
    }

    luaL_error(L, "invalid number of arguments in seal2d_new_bmfont"
               " expected 2, but got %d", n);
    return 0;
}

int lseal2d_bmfont_set_text(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2){
        bmfont* b = (bmfont*)lua_touserdata(L, 1);
        const char* text = luaL_checkstring(L, 2);
        b->set_text(text);
        return 1;
    }

    luaL_error(L, "invalid number of arguments in seal2d_new_bmfont"
               " expected 2, but got %d", n);
    return 0;
}

int luaopen_seal2d_bmfont(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lseal2d_new_bmfont },
        { "set_text",       lseal2d_bmfont_set_text   },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    lua_context::stackDump(L);
    return 1;
}

NS_S2D_END
