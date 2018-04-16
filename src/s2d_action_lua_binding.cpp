#include "s2d_action_lua_binding.h"
#include "s2d_action.h"

#if (S2D_ENABLE_LUA == 1)

NS_S2D

static int lua_seal2d_action_move_to(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 3) {
        lua_Number duration = luaL_checknumber(L, 1);
        lua_Number x = luaL_checknumber(L, 2);
        lua_Number y = luaL_checknumber(L, 3);

        action_move* a = new action_move();
        a->init(duration, x, y);

        lua_pushlightuserdata(L, a);
        return 1;
    } else {
        luaL_error(L, "invalid number of arguments for lua_seal2d_action_move_to, expected %d, got %d", 3, n);
        return 0;
    }
}

static int lua_seal2d_action_scale_to(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 3) {
        lua_Number duration = luaL_checknumber(L, 1);
        lua_Number x = luaL_checknumber(L, 2);
        lua_Number y = luaL_checknumber(L, 3);

        action_scale_to* a = new action_scale_to();
        a->init(duration, x, y);

        lua_pushlightuserdata(L, a);
        return 1;
    } else {
        luaL_error(L, "invalid number of arguments for lua_seal2d_action_scale_to, expected %d, got %d", 3, n);
        return 0;
    }
}

int luaopen_seal2d_action(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "move_to",  lua_seal2d_action_move_to},
        { "scale_to", lua_seal2d_action_scale_to},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

NS_S2D_END

#endif
