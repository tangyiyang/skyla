#include "s2d_sprite_lua_binding.h"
#include "s2d_sprite.h"
#include "s2d_context.h"

NS_S2D

static int lseal2d_sprite_new(lua_State* L)
{
    int n = lua_gettop(L);

    sprite* s = nullptr;
    if (n == 1) {
        s = new sprite();
        char* file_name = (char*)luaL_checkstring(L, 1);
        if (file_name[0] == '#') {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->init(f);
        } else {
            s->init(file_name);
        }
    } else if (n == 0) {
        s = new sprite();
        s->init();
    } else {
        luaL_error(L, "invalid arguments to seal2d.new_sprite");
        return 0;
    }

    lua_pushlightuserdata(L, s);
    return 1;
}

static int lseal2d_sprite_set_color(lua_State* L)
{
    sprite* s = (sprite*)lua_touserdata(L, 1);
    lua_Integer color = luaL_checkinteger(L, 2);
    s->set_color((uint32_t)color);
    return 0;
}

static int lseal2d_sprite_set_texture(lua_State* L)
{
    int n = lua_gettop(L);
    if (n < 2) {
        luaL_error(L, "invalid number of arguments passed to sprite_set_texture"
                   "expected more than 2, but got %d", n);
        return 0;
    }

    sprite* s = (sprite*)lua_touserdata(L, 1);
    if (n == 2) {
        char* file_name = (char*)luaL_checkstring(L, 2);
        if (file_name[0] == '#' && strlen(file_name) > 2) {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->set_texture(f);
        } else {
            s->set_texture(file_name);
        }
    }

    return 0;
}

static int lseal2d_sprite_set_blend_mode(lua_State* L)
{
    int n = lua_gettop(L);
    if (n != 2) {
        luaL_error(L, "invalid number of arguments passed to sprite_set_blend_mode"
                   "expected more than 2, but got %d", n);
        return 0;
    }
    sprite* s = (sprite*)lua_touserdata(L, 1);
    lua_Integer mode = luaL_checkinteger(L, 2);

    S2D_ASSERT(mode >= 0 && mode < BLEND_MODE_MAX);
    s->set_blend_mode((enum blend_mode)mode);

    return 0;
}

int luaopen_seal2d_sprite(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lseal2d_sprite_new },
        { "set_texture",    lseal2d_sprite_set_texture },
        { "set_color",      lseal2d_sprite_set_color},
        { "set_blend_mode", lseal2d_sprite_set_blend_mode},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

NS_S2D_END
