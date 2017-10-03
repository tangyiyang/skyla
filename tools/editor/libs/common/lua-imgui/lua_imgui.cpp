#include "lua_imgui.h"

#define REGISTER_LIB_FUNC(name) {#name, limgui_##name}

#define IMPLEMENT_CALL_PARAM_VOID_RETURN_BOOL_1(call) \
int limgui_##call(lua_State* L) \
{ \
    lua_pushboolean(L, ImGui::call()); \
    return 1;\
}

#define IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(call) \
int limgui_##call(lua_State* L) \
{ \
    ImGui::call(); \
    return 0;\
}

#define ARRAY_SIZE(ARR)  ((int)(sizeof(ARR)/sizeof(*ARR)))

int limgui_GetIO_KeyCtrl(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    lua_pushboolean(L, io.KeyCtrl);
    return 1;
}

int limgui_GetIO_KeySuper(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    lua_pushboolean(L, io.KeySuper);
    return 1;
}

int limgui_GetIO_KeysDown(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    int n = ARRAY_SIZE(io.KeysDown);
    lua_newtable(L);
    for (int i = 0; i < n; ++i) {
        if (io.KeysDownDuration[i] > 0.0f) {

        }
    }
    return 1;
}

int limgui_BeginMenu(lua_State* L)
{
    int n = lua_gettop(L);
    bool ret = false;
    if (n == 1) {
        const char* label = lua_tostring(L, 1);
        ret = ImGui::BeginMenu(label);
    } else if (n == 2) {
        const char* label = lua_tostring(L, 1);
        bool enabled = lua_toboolean(L, 2);
        ret = ImGui::BeginMenu(label, enabled);
    } else {
        luaL_error(L, "invalid args for BeginMenu");
    }

    lua_pushboolean(L, ret);
    return 1;
}

int limgui_MenuItem(lua_State* L)
{
    int n = lua_gettop(L);
    bool ret = false;
    if (n == 1) {
        ret = ImGui::MenuItem(lua_tostring(L, 1));
        lua_pushboolean(L, ret);
        return 1;
    } else if (n == 2) {
        ret = ImGui::MenuItem(lua_tostring(L, 1), lua_tostring(L, 2));
        lua_pushboolean(L, ret);
        return 1;
    } else if (n == 3) {
        const char* label = lua_tostring(L, 1);
        const char* shortcut = "";
        if (!lua_isnil(L, 2)) {
            shortcut = lua_tostring(L, 2);
        }
        bool selected = lua_toboolean(L, 3);
        bool* p = &selected;
        ret = ImGui::MenuItem(label, shortcut, p);
        lua_pushboolean(L, ret);
        lua_pushboolean(L, *p);
        return 2;
    } else if (n == 4) {
        const char* label = lua_tostring(L, 1);
        const char* shortcut = lua_tostring(L, 2);
        if (!lua_isnil(L, 2)) {
            shortcut = lua_tostring(L, 2);
        }
        bool selected = lua_toboolean(L, 3);
        bool enabled = lua_toboolean(L, 4);
        bool* p = &selected;
        ret = ImGui::MenuItem(label, shortcut, p, enabled);
        lua_pushboolean(L, ret);
        lua_pushboolean(L, *p);
        return 2;
    } else {
        luaL_error(L, "invalid args for MenuItem");
    }
    return 0;
}

IMPLEMENT_CALL_PARAM_VOID_RETURN_BOOL_1(BeginMainMenuBar);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(EndMainMenuBar);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(Separator);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(EndMenu);

int luaopen_imgui_core(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "io.keyCtrl", limgui_GetIO_KeyCtrl},
        { "io.KeySuper", limgui_GetIO_KeySuper},
        { "io.KeysDown", limgui_GetIO_KeysDown},

        REGISTER_LIB_FUNC(BeginMenu),
        REGISTER_LIB_FUNC(EndMenu),
        REGISTER_LIB_FUNC(BeginMainMenuBar),
        REGISTER_LIB_FUNC(EndMainMenuBar),
        REGISTER_LIB_FUNC(MenuItem),
        REGISTER_LIB_FUNC(Separator),
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}
