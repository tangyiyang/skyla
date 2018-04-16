#include "s2d_lua_context.h"
#include "s2d_util.h"
#include "s2d_sprite.h"
#include "s2d_panel.h"
#include "s2d_action.h"
#include "s2d_context.h"

#include "s2d_node_lua_binding.h"
#include "s2d_sprite_lua_binding.h"
#include "s2d_bmfont_lua_binding.h"
#include "s2d_action_lua_binding.h"
#include "s2d_timer_lua_binding.h"
#include "s2d_context_lua_binding.h"
#include "s2d_primitive_node_lua_binding.h"

#if (S2D_ENABLE_LUA == 1)

#ifdef __cplusplus
extern "C" {
extern int luaopen_cjson(lua_State* L);
}
#endif

NS_S2D

// TODO: add the binding type checking, we should consider the inheritance checking, sprite is valid for node.
static bool type_valid(lua_State* L, const char* expected)
{
#if ENABLE_TYPE_CHECK
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, "__type");
    const char* type = lua_tostring(L, -1);
    if (strcmp(type, expected) != 0) {
        luaL_error(L, "lseal2d_sprite_set_pos, require: %s, but got: %s", expected, type);
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

    lua_getfield(L, -1, "on_start");
    lua_setfield(L, LUA_REGISTRYINDEX, CONTEXT_START);
    lua_getfield(L, -1, "on_update");
    lua_setfield(L, LUA_REGISTRYINDEX, CONTEXT_UPDATE);
    lua_getfield(L, -1, "on_destory");
    lua_setfield(L, LUA_REGISTRYINDEX, CONTEXT_DESTROY);

    STACK_DUMP(L);
    lua_pop(L, -1);
    return 0;
}

static int lseal2d_new_panel(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 4) {
        lua_Number x = luaL_checknumber(L, 1);
        lua_Number y = luaL_checknumber(L, 2);
        lua_Number w = luaL_checknumber(L, 3);
        lua_Number h = luaL_checknumber(L, 4);

        panel* p = new panel();
        p->init(x, y, w, h);
        lua_pushlightuserdata(L, p);
        return 1;
    }

    luaL_error(L, "invalid number of arguments in lseal2d_new_panel"
               " expected 4, but got %d", n);
    return 0;
}

int luaopen_seal2d_panel(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lseal2d_new_panel },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    lua_context::stackDump(L);
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

static int luaopen_seal2d_util(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "add_search_path", lseal2d_add_search_path },
        { "load_file",  lseal2d_util_load_file },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

int lseal2d_context(lua_State* L)
{
    static context* C = context::C();
    lua_pushlightuserdata(L, C);
    return 1;
}

static int luaopen_seal2d(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "inject",  lseal2d_inject     },
        { "get_context", lseal2d_context    },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

void lua_context::stackDump (lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                LOGD_NO_NEW_LINE("%s ", lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                LOGD_NO_NEW_LINE("%s ", lua_toboolean(L, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                LOGD_NO_NEW_LINE("%g ", lua_tonumber(L, i));
                break;

            default:  /* other values */
                LOGD_NO_NEW_LINE("%s ", lua_typename(L, t));
                break;
        }
    }
    LOGD_NO_NEW_LINE("\n");
}

int lua_context::call_lua(lua_State* L, int n, int r)
{
    int err = lua_pcall(L, n, r, 0);
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
            LOGE("!LUA_ERRERR : %s\n", lua_tostring(L, -1));
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

void lua_context::register_lua_extensions(lua_State* L)
{
    luaL_Reg lua_modules[] = {
        { "cjson",             luaopen_cjson            },
        { "seal2d",            luaopen_seal2d           },
        { "seal2d_node",       luaopen_seal2d_node      },
        { "seal2d_sprite",     luaopen_seal2d_sprite    },
        { "seal2d_primitive",  luaopen_seal2d_primitive },
        { "seal2d_bmfont",     luaopen_seal2d_bmfont    },
        { "seal2d_panel",      luaopen_seal2d_panel     },
        { "seal2d_context",    luaopen_seal2d_context   },
        { "seal2d_util",       luaopen_seal2d_util      },
        { "seal2d_timer",      luaopen_seal2d_timer     },
        { "seal2d_action",     luaopen_seal2d_action    },

        { NULL, NULL}
    };

    luaL_Reg* lib = lua_modules;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++) {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }

    lua_pop(L, 2);
}

void lua_context::init()
{
    lua_State* L = luaL_newstate();

    luaL_openlibs(L);

    register_lua_extensions(L);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_FUNC_TABLE);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_TIMER_TABLE);

    assert(lua_gettop(L) == 0);

    _lua_state = L;
}

void lua_context::on_start(context* ctx, const char* script_path)
{
    lua_State* L = _lua_state;

    file_entry* f = util::load_file(script_path, false);
    int r = luaL_loadbufferx(L, (const char*)f->_buffer, f->_size, script_path, NULL);
    if (r != LUA_OK) {
        LOGE("error load lua file, %s\n");
        exit(-1);
    }
    call_lua(L, 0, LUA_MULTRET);

    lua_getfield(L, LUA_REGISTRYINDEX, CONTEXT_START);
    call_lua(_lua_state, 0, 0);
}

void lua_context::on_update(float dt)
{
    lua_getfield(_lua_state, LUA_REGISTRYINDEX, CONTEXT_UPDATE);
    lua_pushnumber(_lua_state, dt);
    call_lua(_lua_state, 1, 0);
}

void lua_context::on_destroy()
{
    lua_getfield(_lua_state, LUA_REGISTRYINDEX, CONTEXT_DESTROY);
    call_lua(_lua_state, 0, 0);
}

#endif

NS_S2D_END
