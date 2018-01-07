#include "entry.h"
#include "lua_editor.h"

#include "level_editor_common.h"
#include "lua_imgui.h"
#include "lfs.h"

static void register_custom_lua_modules(lua_State* L)
{
    luaL_Reg lua_modules[] = {
        { "editor.core", luaopen_editor_core},
        { "imgui", luaopen_imgui_core},
        { "lfs", luaopen_lfs},
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

void entry::on_init(context* ctx)
{
    register_custom_lua_modules(ctx->_lua_context->_lua_state);

#if S2D_ENABLE_LUA == 1
    ctx->_lua_context->on_start(ctx, "res/scripts/main.lua");
#endif
}

void entry::on_pause()
{
    LOGD("entry:: on_pause");
}

void entry::on_resume()
{
    LOGD("entry:: on_resume");
}

void entry::on_destroy()
{
    LOGD("entry:: on_destroy");
}

void entry::on_resize(context* ctx)
{
    LOGD("entry:: on_resize");
}

void entry::on_begin_update(float dt)
{

}

void entry::on_after_update(float dt)
{

}
