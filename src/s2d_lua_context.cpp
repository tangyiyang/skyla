/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#include "s2d_lua_context.h"
#include "s2d_util.h"
#include "s2d_sprite.h"
#include "s2d_context.h"

#ifdef S2D_ENABLE_LUA

#define SEAL2D_TYPE_SPRITE "seal2d.sprite"

NS_S2D

#define TRACE_BACK_FUNC_INDEX (1)

static bool type_valid(lua_State* L, const char* expected)
{
#if ENABLE_TYPE_CHECK
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, "__type");
    const char* type = lua_tostring(L, -1);
    if (strcmp(type, SEAL2D_TYPE_SPRITE) != 0) {
        luaL_error(L, "lseal2d_sprite_set_pos, require: seal2d.sprite, but got: %s", type);
        return false;
    }
    lua_pop(L, 2);
    return true;
#endif
}

static int lseal2d_inject(lua_State* L)
{
    if(!lua_istable(L, -1)) {
        LOGE("seal2d lua require a functions of {on_init, on_update, on_destroy} to start. :)");
        exit(1);
    }

    lua_getfield(L, -1, "on_init");
    lua_setfield(L, LUA_REGISTRYINDEX, CONTEXT_INIT);

    lua_getfield(L, -1, "on_update");
    lua_setfield(L, LUA_REGISTRYINDEX, CONTEXT_UPDATE);

    lua_getfield(L, -1, "on_destory");
    lua_setfield(L, LUA_REGISTRYINDEX, CONTEXT_DESTROY);

    lua_pop(L, -1);
    return 0;
}

static int lseal2d_sprite_set_pos(lua_State* L)
{
    sprite* s = (sprite*)lua_touserdata(L, 1);

    S2D_ASSERT(type_valid(L, SEAL2D_TYPE_SPRITE));

    lua_Number x = luaL_checknumber(L, 2);
    lua_Number y = luaL_checknumber(L, 3);
    s->set_pos(x, y);
    return 0;
}

static int lseal2d_new_sprite(lua_State* L)
{
    char* file_name = (char*)luaL_checkstring(L, 1);

    sprite* s = new sprite();
    if (file_name[0] == '#') {
        sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
        s->init(f);
    } else {
        s->init(file_name);
    }

    lua_pushlightuserdata(L, s);
    if (luaL_newmetatable(L, "SEAL2D_SPRITE")) {
        luaL_Reg l[] = {
            { "set_pos", lseal2d_sprite_set_pos },
            { NULL, NULL },
        };
        luaL_newlib(L, l);
        lua_setfield(L, -2, "__index");

        lua_pushstring(L, "seal2d.sprite");
        lua_setfield(L, -2, "__type");
    }
    lua_setmetatable(L, -2);
    return 1;
}

#ifdef __cplusplus

extern "C" {

    int luaopen_seal2d(lua_State* L)
    {
    #ifdef luaL_checkversion
        luaL_checkversion(L);
    #endif
        luaL_Reg lib[] = {
            { "inject", lseal2d_inject},
            { "sprite", lseal2d_new_sprite},
            { NULL, NULL },
        };

        luaL_newlib(L, lib);
        return 1;
    }

#endif
}

void lua_context::stackDump (lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                LOGD_NO_RETURN("%s ", lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                LOGD_NO_RETURN("%s ", lua_toboolean(L, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                LOGD_NO_RETURN("%g ", lua_tonumber(L, i));
                break;

            default:  /* other values */
                LOGD_NO_RETURN("%s ", lua_typename(L, t));
                break;
        }
    }
    LOGD_NO_RETURN("\n");
}

static int traceback (lua_State* L)
{
    const char *msg = lua_tostring(L, 1);
    if (msg) {
        luaL_traceback(L, L, msg, 1);
    } else if (!lua_isnoneornil(L, 1)) {
        if (!luaL_callmeta(L, 1, "__tostring")) {
            lua_pushliteral(L, "(no error message)");
        }
    }
    return 1;
}

int lua_context::call_lua(lua_State* L, int n, int r)
{
    int err = lua_pcall(L, n, r, TRACE_BACK_FUNC_INDEX);
    switch(err) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            LOGE("!LUA_ERRRUN : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        case LUA_ERRMEM:
            LOGE("!LUA_ERRMEM : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        case LUA_ERRERR:
            LOGE("!LUA_ERRERR : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        case LUA_ERRGCMM:
            LOGE("!LUA_ERRRUN : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        default:
            LOGE("!Unknown Lua error : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
    }
    return err;
}

void lua_context::register_lua_extensions()
{
    luaL_Reg lua_modules[] = {
        { "seal2d",        luaopen_seal2d },
        { NULL, NULL}
    };

    luaL_Reg* lib = lua_modules;
    lua_getglobal(_lua_state, "package");
    lua_getfield(_lua_state, -1, "preload");
    for (; lib->func; lib++) {
        lua_pushcfunction(_lua_state, lib->func);
        lua_setfield(_lua_state, -2, lib->name);
    }

    lua_pop(_lua_state, 2);
}

void lua_context::init()
{
    _lua_state = nullptr;
    _lua_state = luaL_newstate();
    lua_assert(_lua_state);

    luaL_openlibs(_lua_state);

    register_lua_extensions();

    assert(lua_gettop(_lua_state) == 0);
    lua_pushcfunction(_lua_state, traceback);
}

void lua_context::on_start(context* ctx, const char* script_path)
{
    int r = luaL_loadfile(_lua_state, script_path);
    if (r != LUA_OK) {
        LOGE("error load lua file\n");
        return;
    }
    call_lua(_lua_state, 0, LUA_MULTRET);

    lua_getfield(_lua_state, LUA_REGISTRYINDEX, CONTEXT_INIT);

//    lua_pushlightuserdata(L, ctx);

    call_lua(_lua_state, 0, 0);
}

void lua_context::on_update(uint32_t dt)
{
    lua_getfield(_lua_state, LUA_REGISTRYINDEX, CONTEXT_UPDATE);
    lua_pushinteger(_lua_state, dt);
    call_lua(_lua_state, 1, 0);
}

void lua_context::on_destroy()
{
    lua_getfield(_lua_state, LUA_REGISTRYINDEX, CONTEXT_DESTROY);
    call_lua(_lua_state, 0, 0);
}

NS_S2D_END

#endif
