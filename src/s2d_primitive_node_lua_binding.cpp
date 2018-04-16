#include "s2d_primitive_node.h"
#include "s2d_primitive_node_lua_binding.h"

#if (S2D_ENABLE_LUA == 1)

NS_S2D

int lseal2d_primitive_new(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 0) {
        primitive_node* p = new primitive_node();
        p->init();
        lua_pushlightuserdata(L, p);
    } else {
        luaL_error(L, "invalid number of args for seal2d.primitive.new, expected 0, got %d", n);
    }
    return 1;
}

int lseal2d_primitive_add_line(lua_State* L)
{
    int n = lua_gettop(L);

    if (n == 6) {
        lua_getfield(L, 1, "__cobj");
        primitive_node* p = (primitive_node*)lua_touserdata(L, -1);
        lua_Number begin_x = luaL_checknumber(L, 2);
        lua_Number begin_y = luaL_checknumber(L, 3);
        lua_Number end_x = luaL_checknumber(L, 4);
        lua_Number end_y = luaL_checknumber(L, 5);
        lua_Integer color = luaL_checkinteger(L, 6);

        LOGD("color = %x", color);
        p->add_line(vec2::make(begin_x, begin_y), vec2::make(end_x, end_y), color);
    } else {
        luaL_error(L, "invalid number of args for seal2d.primitive.add_line, expected 6, got %d", n);
    }
    return 0;
}

int lseal2d_primitive_add_rect(lua_State* L)
{
    return 0;
}

int luaopen_seal2d_primitive(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lseal2d_primitive_new },
        { "add_line",       lseal2d_primitive_add_line },
        { "add_rect",       lseal2d_primitive_add_rect },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

NS_S2D_END

#endif
