#include "bmfont_lua_binding.h"
#include "game_objects/bmfont.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int lskyla_new_bmfont(lua_State* L)
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

    luaL_error(L, "invalid number of arguments in skyla_new_bmfont"
               " expected 2, but got %d", n);
    return 0;
}

int lskyla_bmfont_set_text(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    bmfont* b = (bmfont*)lua_touserdata(L, -1);
    const char* text = luaL_checkstring(L, 2);
    b->set_text(text);

    return 0;
}

int luaopen_skyla_bmfont(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lskyla_new_bmfont },
        { "set_text",       lskyla_bmfont_set_text   },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    lua_context::stackDump(L);
    return 1;
}

NS_SKYLA_END

#endif
