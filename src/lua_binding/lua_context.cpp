#include "lua_context.h"
#include "core/util.h"
#include "game_objects/sprite.h"
#include "game_objects/panel.h"
#include "action.h"
#include "skyla_context.h"

#include "action_lua_binding.h"
#include "bmfont_lua_binding.h"
#include "context_lua_binding.h"
#include "node_lua_binding.h"
#include "primitive_node_lua_binding.h"
#include "sprite_lua_binding.h"
#include "scale9sprite_lua_binding.h"
#include "spine_lua_binding.h"
#include "timer_lua_binding.h"
#include "util_lua_binding.h"

#if (SKYLA_ENABLE_LUA == 1)

#ifdef __cplusplus
extern "C" {
extern int luaopen_cjson(lua_State* L);
}
#endif

NS_SKYLA

static int lskyla_inject(lua_State* L)
{
    if(!lua_istable(L, -1)) {
        LOGE("skyla lua require a functions of {on_init, on_update, on_destroy} to start. :)");
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

static int lskyla_new_panel(lua_State* L)
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

    luaL_error(L, "invalid number of arguments in lskyla_new_panel"
               " expected 4, but got %d", n);
    return 0;
}

int luaopen_skyla_panel(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lskyla_new_panel },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    lua_context::stackDump(L);
    return 1;
}

int lskyla_context(lua_State* L)
{
    static context* C = context::C();
    lua_pushlightuserdata(L, C);
    return 1;
}

static int luaopen_skyla(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "inject",  lskyla_inject     },
        { "get_context", lskyla_context    },
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


/*
 ** Message handler used to run all chunks,
 *  this function is copied from lua.c
 */
static int msghandler (lua_State *L) {
    const char *msg = lua_tostring(L, 1);
    if (msg == NULL) {  /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
            return 1;  /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                                  luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}

int lua_context::call_lua(lua_State* L, int n, int r)
{
    int base = lua_gettop(L) - n;  /* function index */
    lua_pushcfunction(L, msghandler);  /* push message handler */
    lua_insert(L, base);  /* put it under function and args */
    int err = lua_pcall(L, n, r, base);
    switch(err) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            LOGE("!LUA_ERRRUN : %s\n", lua_tostring(L,-1));
            SKYLA_ASSERT(false);
            break;
        case LUA_ERRMEM:
            LOGE("!LUA_ERRMEM : %s\n", lua_tostring(L,-1));
            SKYLA_ASSERT(false);
            break;
        case LUA_ERRERR:
            LOGE("!LUA_ERRERR : %s\n", lua_tostring(L, -1));
            SKYLA_ASSERT(false);
            break;
        case LUA_ERRGCMM:
            LOGE("!LUA_ERRRUN : %s\n", lua_tostring(L,-1));
            SKYLA_ASSERT(false);
            break;
        default:
            LOGE("!Unknown Lua error : %s\n", lua_tostring(L,-1));
            SKYLA_ASSERT(false);
            break;
    }
    lua_remove(L, base);  /* remove message handler from the stack */
    return err;
}

void lua_context::register_lua_extensions(lua_State* L)
{
    luaL_Reg lua_modules[] = {
        { "cjson",               luaopen_cjson               },
        { "skyla",              luaopen_skyla              },
        { "skyla_node",         luaopen_skyla_node         },
        { "skyla_sprite",       luaopen_skyla_sprite       },
        { "skyla_scale9sprite", luaopen_skyla_scale9sprite },
        { "skyla_spine",        luaopen_skyla_spine        },
        { "skyla_primitive",    luaopen_skyla_primitive    },
        { "skyla_bmfont",       luaopen_skyla_bmfont       },
        { "skyla_panel",        luaopen_skyla_panel        },
        { "skyla_context",      luaopen_skyla_context      },
        { "skyla_util",         luaopen_skyla_util         },
        { "skyla_timer",        luaopen_skyla_timer        },
        { "skyla_action",       luaopen_skyla_action       },

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

    register_user_table(SKYLA_TOUCH_FUNC_TABLE);
    register_user_table(SKYLA_UPDATE_FUNC_TABLE);
    register_user_table(SKYLA_USER_TIMER_TABLE);

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


NS_SKYLA_END

#endif
