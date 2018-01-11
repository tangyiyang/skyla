#include "s2d_context_lua_binding.h"
#include "s2d_context.h"

NS_S2D

static int lseal2d_context_get_root(lua_State* L)
{
    context* C = context::C();
    lua_pushlightuserdata(L, C->get_root());
    return 1;
}

static int lseal2d_context_get_visible_rect(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    context* C = (context*)lua_touserdata(L, -1);

    rect* r = &(C->_visible_rect);
    lua_newtable(L);
    lua_pushnumber(L, r->origin.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, r->origin.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, r->size.width);
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, r->size.height);
    lua_setfield(L, -2, "height");

    return 1;
}

int luaopen_seal2d_context(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "get_root",         lseal2d_context_get_root },
        { "get_visible_rect", lseal2d_context_get_visible_rect },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

NS_S2D_END

