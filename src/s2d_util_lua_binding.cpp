#include "s2d_util_lua_binding.h"

#include "s2d_util.h"
#include "s2d_context.h"

#if (S2D_ENABLE_LUA == 1)

NS_S2D

static int lsea2d_util_get_platform(lua_State* L)
{
    lua_pushstring(L, util::get_platform());
    return 1;
}

int lseal2d_add_search_path(lua_State* L)
{
    const char* path = luaL_checkstring(L, 1);
    context::C()->_file_system->add_search_path(path);
    return 1;
}

int lseal2d_util_load_file(lua_State* L)
{
    const char* file_path = luaL_checkstring(L, 1);
    bool cached = lua_toboolean(L, 2);

    file_entry* f = util::load_file(file_path, cached);

    lua_pushlstring(L, (const char*)f->_buffer, f->_size);
    return 1;
}

int lseal2d_get_sandbox_path(lua_State* L)
{
    lua_pushstring(L, context::C()->_file_system->get_sandbox_path().c_str());
    return 1;
}

int luaopen_seal2d_util(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "get_platform", lsea2d_util_get_platform },
        { "add_search_path", lseal2d_add_search_path },
        { "get_sandbox_path", lseal2d_get_sandbox_path },
        { "load_file",  lseal2d_util_load_file },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

NS_S2D_END

#endif

