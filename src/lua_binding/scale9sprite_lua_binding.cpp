#include "scale9sprite_lua_binding.h"
#include "game_objects/scale9sprite.h"
#include "skyla_context.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int lua_skyla_scale9sprite_new(lua_State* L)
{
    int n = lua_gettop(L);

    scale9sprite* s = nullptr;
    if (n == 1) {
        s = new scale9sprite();
        s->init();
        char* file_name = (char*)luaL_checkstring(L, 1);
        if (file_name[0] == '#') {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->set_texture_with_frame(f);
        } else {
            s->set_texture_with_file(file_name);
        }
    } else if (n == 0) {
        s = new scale9sprite();
        s->init();
    } else {
        luaL_error(L, "invalid arguments to skyla.new_sprite, expected 0 or 1, but got %d", n);
        return 0;
    }

    lua_pushlightuserdata(L, s);
    return 1;
}

static int lskyla_scale9sprite_set_border(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    scale9sprite* s = (scale9sprite*)lua_touserdata(L, -1);
    lua_Number left = luaL_checknumber(L, 2);
    lua_Number right = luaL_checknumber(L, 3);
    lua_Number bottom = luaL_checknumber(L, 4);
    lua_Number top = luaL_checknumber(L, 5);
    s->set_border(left, right, bottom, top);

    return 0;
}

static int lsea2d_scale9sprite_set_size(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    scale9sprite* s = (scale9sprite*)lua_touserdata(L, -1);
    lua_Number width = luaL_checknumber(L, 2);
    lua_Number height = luaL_checknumber(L, 3);
    s->set_size(width, height);
    return 0;
}

int luaopen_skyla_scale9sprite(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",      lua_skyla_scale9sprite_new },
        { "set_border", lskyla_scale9sprite_set_border },
        { "set_size", lsea2d_scale9sprite_set_size },

        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}


NS_SKYLA_END

#endif

