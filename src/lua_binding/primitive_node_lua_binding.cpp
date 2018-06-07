#include "primitive_node_lua_binding.h"

#include "game_objects/primitive_node.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int lskyla_primitive_new(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 0) {
        primitive_node* p = new primitive_node();
        p->init();
        lua_pushlightuserdata(L, p);
    } else {
        luaL_error(L, "invalid number of args for skyla.primitive.new, expected 0, got %d", n);
    }
    return 1;
}

int lskyla_primitive_add_line(lua_State* L)
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

        p->add_line(vec2::make(begin_x, begin_y), vec2::make(end_x, end_y), color);
    } else {
        luaL_error(L, "invalid number of args for skyla.primitive.add_line, expected 6, got %d", n);
    }
    return 0;
}

int lskyla_primitive_add_rect(lua_State* L)
{ int n = lua_gettop(L);

    if (n == 6) {
        lua_getfield(L, 1, "__cobj");
        primitive_node* p = (primitive_node*)lua_touserdata(L, -1);
        lua_Number left = luaL_checknumber(L, 2);
        lua_Number bottom = luaL_checknumber(L, 3);
        lua_Number width = luaL_checknumber(L, 4);
        lua_Number height = luaL_checknumber(L, 5);
        lua_Integer color = luaL_checkinteger(L, 6);

        p->add_rect(vec2::make(left, bottom), size::make(width, height), color);
    } else {
        luaL_error(L, "invalid number of args for skyla.primitive.add_rect, expected 6, got %d", n);
    }
    return 0;
    return 0;
}

int luaopen_skyla_primitive(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lskyla_primitive_new },
        { "add_line",       lskyla_primitive_add_line },
        { "add_rect",       lskyla_primitive_add_rect },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

NS_SKYLA_END

#endif
