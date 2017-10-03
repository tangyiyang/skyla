#include <assert.h>

#include "level_editor_common.h"
#include "lua_handler.h"

extern lua_State* lState;
extern void LoadImguiBindings();

NS_GAME_TOOL

#define TRACE_BACK_FUNC_INDEX (1)

void lua_handler::stackDump (lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("%s ", lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                printf("%s ", lua_toboolean(L, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                printf("%g ", lua_tonumber(L, i));
                break;

            default:  /* other values */
                printf("%s ", lua_typename(L, t));
                break;
        }
    }
    printf("\n");
}

static int traceback (lua_State *L)
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

int lua_handler::call_lua(lua_State *L, int n, int r)
{
    int err = lua_pcall(L, n, r, TRACE_BACK_FUNC_INDEX);
    switch(err) {
        case LUA_OK:
            break;
        case LUA_ERRRUN:
            printf("!LUA_ERRRUN : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        case LUA_ERRMEM:
            printf("!LUA_ERRMEM : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        case LUA_ERRERR:
            printf("!LUA_ERRERR : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        case LUA_ERRGCMM:
            printf("!LUA_ERRRUN : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
        default:
            printf("!Unknown Lua error : %s\n", lua_tostring(L,-1));
            assert(false);
            break;
    }
    return err;
}

void lua_handler::register_lua_extensions()
{
    luaL_Reg lua_modules[] = {
        { "editor.core", luaopen_editor_core},
        { "cjson", luaopen_cjson},
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

lua_handler::lua_handler()
{
    _lua_state = nullptr;
    _lua_state = luaL_newstate();
    lua_assert(_lua_state);

    luaL_openlibs(_lua_state);

    register_lua_extensions();

    // register the imgui binding
    lState = _lua_state;
    LoadImguiBindings();

    assert(lua_gettop(_lua_state) == 0);
    lua_pushcfunction(_lua_state, traceback);

    int r = luaL_loadfile(_lua_state, "./res/scripts/main.lua");
    if (r != LUA_OK) {
        printf("error load lua file\n");
        return;
    }
    call_lua(_lua_state, 0, LUA_MULTRET);
}

lua_handler::~lua_handler()
{

}

void lua_handler::init()
{
    lua_getfield(_lua_state, LUA_REGISTRYINDEX, EDITOR_INIT);
    call_lua(_lua_state, 0, 0);
}

void lua_handler::update(uint32_t dt)
{
    lua_getfield(_lua_state, LUA_REGISTRYINDEX, EDITOR_UPDATE);
    lua_pushinteger(_lua_state, dt);
    call_lua(_lua_state, 1, 0);
}

NS_GAME_TOOL_END
