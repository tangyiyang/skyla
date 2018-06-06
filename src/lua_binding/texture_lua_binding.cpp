#include "texture_lua_binding.h"

#include "skyla_context.h"
#include "renderer/texture.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

int lskyla_new_texture(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 1) {
        const char* path = luaL_checkstring(L, 1);
        texture* t = skyla::context::C()->_texture_cache->load(path);
        lua_pushlightuserdata(L, t);
        return 1;
    }

    luaL_error(L, "invalid number of arguments in skyla_new_texture"
               " expected 1, but got %d", n);
    return 0;
}

int lskyla_get_opengl_id(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    texture* b = (texture*)lua_touserdata(L, -1);

    lua_pushinteger(L, b->_name);
    return 1;
}

int lskyla_get_file_path(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    texture* b = (texture*)lua_touserdata(L, -1);

    lua_pushstring(L, b->get_file_path().c_str());
    return 1;
}

int luaopen_skyla_texture(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",                    lskyla_new_texture },
        { "get_opengl_id",          lskyla_get_opengl_id },
        { "get_file_path",          lskyla_get_file_path },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

NS_SKYLA_END

#endif
