#include "sprite_lua_binding.h"

#include "game_objects/sprite.h"
#include "skyla_context.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

static int lskyla_sprite_frame_cache_load(lua_State* L)
{
    int n = lua_gettop(L);

    if (n == 2) {
        const char* json_atlas = luaL_checkstring(L, 1);
        const char* texture_file = luaL_checkstring(L, 2);
        context::C()->_sprite_frame_cache->load(json_atlas, texture_file);
    } else {
        luaL_error(L, "invalid number of arguments, expected 2, got = %d", n);
    }
    return 0;
}

static int lskyla_sprite_new(lua_State* L)
{
    int n = lua_gettop(L);

    sprite* s = nullptr;
    if (n == 1) {
        s = new sprite();
        s->init();
        char* file_name = (char*)luaL_checkstring(L, 1);
        if (file_name[0] == '#') {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->set_texture_with_frame(f);
        } else {
            s->set_texture_with_file(file_name);
        }
    } else if (n == 0) {
        s = new sprite();
        s->init();
    } else {
        luaL_error(L, "invalid arguments to skyla.new_sprite, expected 0 or 1, but got %d", n);
        return 0;
    }

    lua_pushlightuserdata(L, s);
    return 1;
}

static int lskyla_sprite_set_color(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    sprite* s = (sprite*)lua_touserdata(L, -1);
    lua_Integer color = luaL_checkinteger(L, 2);
    s->set_color((uint32_t)color);
    return 0;
}

static int lskyla_sprite_set_texture(lua_State* L)
{
    int n = lua_gettop(L);
    if (n < 2) {
        luaL_error(L, "invalid number of arguments passed to sprite_set_texture"
                   "expected more than 2, but got %d", n);
        return 0;
    }

    if (n == 2) {
        lua_getfield(L, 1, "__cobj");
        sprite* s = (sprite*)lua_touserdata(L, -1);
        char* file_name = (char*)luaL_checkstring(L, 2);
        if (file_name[0] == '#' && strlen(file_name) > 2) {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->set_texture_with_frame(f);
        } else {
            s->set_texture_with_file(file_name);
        }
    }

    return 0;
}

static int lskyla_sprite_get_texture(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    sprite* s = (sprite*)lua_touserdata(L, -1);

    lua_pushlightuserdata(L, s->get_texture());
    return 1;
}

static int lskyla_sprite_set_blend_mode(lua_State* L)
{
    int n = lua_gettop(L);
    if (n != 2) {
        luaL_error(L, "invalid number of arguments passed to sprite_set_blend_mode"
                   "expected more than 2, but got %d", n);
        return 0;
    }
    lua_getfield(L, 1, "__cobj");
    sprite* s = (sprite*)lua_touserdata(L, -1);
    lua_Integer mode = luaL_checkinteger(L, 2);

    SKYLA_ASSERT(mode >= 0 && mode < BLEND_MODE_MAX);
    s->set_blend_mode((enum blend_mode)mode);

    return 0;
}

int luaopen_skyla_sprite(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",                lskyla_sprite_new },
        { "load_frames",        lskyla_sprite_frame_cache_load },
        { "set_texture",        lskyla_sprite_set_texture },
        { "set_color",          lskyla_sprite_set_color},
        { "set_blend_mode",     lskyla_sprite_set_blend_mode},
        { "get_texture",        lskyla_sprite_get_texture},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

NS_SKYLA_END

#endif
