#include "s2d_timer_lua_binding.h"
#include "s2d_timer.h"

NS_S2D

static int lua_seal2d_timer_new(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        timer* t = new timer();

        float interval = luaL_checknumber(L, 1);

        lua_getfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_TIMER_TABLE);
        lua_pushlightuserdata(L, t);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);

        t->init(interval, [=](void*){
            lua_getfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_TIMER_TABLE); /* event_table */
            lua_pushlightuserdata(L, t); /* event_table node */
            lua_gettable(L, -2);    /* event_table node function */

            lua_context::call_lua(L, 0, 0);
            lua_pop(L, -1);
        }, nullptr);

        lua_pop(L, -1);

        lua_pushlightuserdata(L, t);
        return 1;
    }
    luaL_error(L, "error args passed to lua_seal2d_timer_new"
               "expected 3, but got %d", n);
    return 0;
}

static int lua_seal2d_timer_start(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    timer* C = (timer*)lua_touserdata(L, -1);
    C->start();
    return 0;
}

static int lua_seal2d_timer_stop(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    timer* C = (timer*)lua_touserdata(L, -1);
    C->stop();
    return 0;
}

int luaopen_seal2d_timer(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",  lua_seal2d_timer_new },
        { "start", lua_seal2d_timer_start },
        { "stop", lua_seal2d_timer_stop },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

NS_S2D_END
