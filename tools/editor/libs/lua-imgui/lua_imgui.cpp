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

#define IMPLEMENT_CALL_IMVEC2_RETURN_VOID(call) \
int limgui_##call(lua_State* L) \
{ \
    lua_Number x = luaL_checknumber(L, 1);\
    lua_Number y = luaL_checknumber(L, 2); \
    ImGui::call(ImVec2(x, y)); \
    return 0; \
}

#define IMPLEMENT_CALL_VOID_RETURN_VEC2(call) \
int limgui_##call(lua_State* L) \
{ \
    ImVec2 v = ImGui::call(); \
    lua_pushnumber(L, v.x); \
    lua_pushnumber(L, v.y); \
    return 2; \
}

IMPLEMENT_CALL_PARAM_VOID_RETURN_BOOL_1(BeginMainMenuBar);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(EndMainMenuBar);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(EndMenu);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(End);
IMPLEMENT_CALL_PARAM_VOID_RETURN_BOOL_1(IsItemActive);
IMPLEMENT_CALL_PARAM_VOID_RETURN_BOOL_1(IsItemClicked);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(Separator);
IMPLEMENT_CALL_PARAM_VOID_RETURN_VOID(TreePop);
IMPLEMENT_CALL_IMVEC2_RETURN_VOID(SetCursorPos);
IMPLEMENT_CALL_VOID_RETURN_VEC2(GetMouseDragDelta);

int limgui_Begin(lua_State* L)
{
    int n = lua_gettop(L);

    bool ret = false;
    if (n == 1) {
        ret = ImGui::Begin(luaL_checkstring(L, 1));
        lua_pushboolean(L, ret);
        return 1;
    } else if (n == 2) {
        bool v = lua_toboolean(L, 2);
        ret = ImGui::Begin(luaL_checkstring(L, 1), &v);
        lua_pushboolean(L, ret);
        lua_pushboolean(L, v);
        return 2;
    } else if (n == 3) {
        bool v = lua_toboolean(L, 2);
        int flags = (int)luaL_checkinteger(L, 3);
        ret = ImGui::Begin(luaL_checkstring(L, 1), &v, flags);
        lua_pushboolean(L, ret);
        lua_pushboolean(L, v);
        return 2;
    } else {
        luaL_error(L, "invalid args for Begin");
    }
    return 0;
}

int limgui_BeginMenu(lua_State* L)
{
    int n = lua_gettop(L);
    bool ret = false;
    if (n == 1) {
        const char* label = luaL_checkstring(L, 1);
        ret = ImGui::BeginMenu(label);
    } else if (n == 2) {
        const char* label = luaL_checkstring(L, 1);
        bool enabled = lua_toboolean(L, 2);
        ret = ImGui::BeginMenu(label, enabled);
    } else {
        luaL_error(L, "invalid args for BeginMenu");
    }

    lua_pushboolean(L, ret);
    return 1;
}

int limgui_Button(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 1) {
        lua_pushboolean(L, ImGui::Button(luaL_checkstring(L, 1)));
        return 1;
    } else if (n == 3) {
        ImVec2 v(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
        lua_pushboolean(L, ImGui::Button(luaL_checkstring(L, 1)));
        return 1;
    } else {
        luaL_error(L, "invalid args for Button");
    }
    return 0;
}

int limgui_CollapsingHeader(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 1) {
        bool ret = ImGui::CollapsingHeader(luaL_checkstring(L, 1));
        lua_pushboolean(L, ret);
        return 1;
    } else if (n == 2) {
        bool ret = ImGui::CollapsingHeader(luaL_checkstring(L, 1), luaL_checkinteger(L, 2));
        lua_pushboolean(L, ret);
        return 1;
    }
    return 0;
}

int limgui_Columns(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 0) {
        ImGui::Columns();
    } else if (n == 1) {
        ImGui::Columns((int)luaL_checkinteger(L, 1));
    } else if (n == 2) {
        ImGui::Columns((int)luaL_checkinteger(L, 1), luaL_checkstring(L, 2));
    } else if (n == 3) {
        ImGui::Columns((int)luaL_checkinteger(L, 1), luaL_checkstring(L, 2), lua_toboolean(L, 3));
    } else {
        luaL_error(L, "invalid args for Columns");
    }
    return 0;
}

int limgui_Checkbox(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        const char* label = luaL_checkstring(L, 1);
        bool v = lua_toboolean(L, 2);
        ImGui::Checkbox(label, &v);
        lua_pushboolean(L, v);
        return 1;
    } else {
        luaL_error(L, "invalid args for Checkbox");
    }

    return 0;
}

int limgui_InputText(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        char buf[512] = "";
        size_t len = 0;
        luaL_checklstring(L, 2, &len);
        strncpy(buf, luaL_checkstring(L, 2), len);
        bool ret = ImGui::InputText(luaL_checkstring(L, 1), buf, 512);
        lua_pushboolean(L, ret);
        lua_pushlstring(L, buf, strlen(buf));
        return 2;
    } else {
        luaL_error(L, "invalid args for InputText");
    }
    return 0;
}

int limgui_Image(lua_State* L)
{
    int n = lua_gettop(L);

    if (n == 3) {
        lua_Integer id = luaL_checkinteger(L, 1);
        lua_Number width = luaL_checknumber(L, 2);
        lua_Number height = luaL_checknumber(L, 3);
        ImGui::Image((ImTextureID)(intptr_t)id, ImVec2(width, height));
    } else {
        luaL_error(L, "invalid args for Image");
    }
    return 0;
}

int limgui_KeyCtrl(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    lua_pushboolean(L, io.KeyCtrl);
    return 1;
}

int limgui_KeySuper(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    lua_pushboolean(L, io.KeySuper);
    return 1;
}

int limgui_KeysDown(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    int n = sizeof(io.KeysDown) / sizeof(*io.KeysDown);
    lua_newtable(L);
    int index = 1;
    for (int i = 0; i < n; ++i) {
        if (io.KeysDownDuration[i] > 0.0f) {
            lua_pushinteger(L, index++);
            lua_pushnumber(L, i);
            lua_settable(L, -3);
        }
    }
    return 1;
}


int limgui_KeysPressed(lua_State* L)
{
    ImGuiIO& io = ImGui::GetIO();
    int n = sizeof(io.KeysDown) / sizeof(*io.KeysDown);
    lua_newtable(L);
    int index = 1;
    for (int i = 0; i < n; ++i) {
        if (ImGui::IsKeyPressed(i, true)) {
            lua_pushinteger(L, index++);
            lua_pushnumber(L, i);
            lua_settable(L, -3);
        }
    }
    return 1;
}

int limgui_MenuItem(lua_State* L)
{
    int n = lua_gettop(L);
    bool ret = false;
    if (n == 1) {
        ret = ImGui::MenuItem(luaL_checkstring(L, 1));
        lua_pushboolean(L, ret);
        return 1;
    } else if (n == 2) {
        ret = ImGui::MenuItem(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
        lua_pushboolean(L, ret);
        return 1;
    } else if (n == 3) {
        const char* label = luaL_checkstring(L, 1);
        const char* shortcut = "";
        if (!lua_isnil(L, 2)) {
            shortcut = luaL_checkstring(L, 2);
        }
        bool selected = lua_toboolean(L, 3);
        bool* p = &selected;
        ret = ImGui::MenuItem(label, shortcut, p);
        lua_pushboolean(L, ret);
        lua_pushboolean(L, *p);
        return 2;
    } else if (n == 4) {
        const char* label = luaL_checkstring(L, 1);
        const char* shortcut = luaL_checkstring(L, 2);
        if (!lua_isnil(L, 2)) {
            shortcut = luaL_checkstring(L, 2);
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

int limgui_SameLine(lua_State* L)
{
    int n = lua_gettop(L);
    if(n == 0) {
        ImGui::SameLine();
    } else if (n == 1) {
        ImGui::SameLine(luaL_checknumber(L, 1));
    } else if (n == 2) {
        ImGui::SameLine(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
    } else {
        luaL_error(L, "invalid args for SameLine");
    }
    return 0;
}

int limgui_SetColumnWidth(lua_State* L)
{
    ImGui::SetColumnWidth(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
    return 0;
}

int limgui_SetWindowSize(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        ImGui::SetWindowSize(ImVec2(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
    } else if (n == 3) {
        ImGui::SetWindowSize(ImVec2(luaL_checknumber(L, 1), luaL_checknumber(L, 2)),
                             (int)luaL_checkinteger(L, 3));
    } else {
        luaL_error(L, "invalid args for SetWindowSize");
    }
    return 0;
}

int limgui_Text(lua_State* L)
{
    // TODO: support var args sometime
    int n = lua_gettop(L);
    if (n == 1) {
        ImGui::Text("%s", luaL_checkstring(L, 1));
    } else {
        luaL_error(L, "invalid args for Text");
    }
    return 0;
}

int limgui_TreeNode(lua_State* L)
{
    lua_pushboolean(L, ImGui::TreeNode(luaL_checkstring(L, 1)));
    return 1;
}

int limgui_TreeNodeEx(lua_State* L)
{
    bool ret = false;
    int n = lua_gettop(L);
    if (n == 1) {
        if (lua_isstring(L, 1)) {
            ret = ImGui::TreeNodeEx(lua_tostring(L, 1));
            lua_pushboolean(L, ret);
            return 1;
        } else {
            luaL_error(L, "invalid args for TreeNodeEx");
            return 0;
        }
    } else if (n == 2) {
        ret = ImGui::TreeNodeEx(luaL_checkstring(L, 1), luaL_checkinteger(L, 2));
        lua_pushboolean(L, ret);
        return 1;
    } else {
        luaL_error(L, "var args for TreeNodeEx not supported yet.");
    }
    return 0;
}

int luaopen_imgui_core(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        REGISTER_LIB_FUNC(Begin),
        REGISTER_LIB_FUNC(BeginMenu),
        REGISTER_LIB_FUNC(BeginMainMenuBar),
        REGISTER_LIB_FUNC(Button),
        REGISTER_LIB_FUNC(Columns),
        REGISTER_LIB_FUNC(CollapsingHeader),
        REGISTER_LIB_FUNC(Checkbox),
        REGISTER_LIB_FUNC(End),
        REGISTER_LIB_FUNC(EndMenu),
        REGISTER_LIB_FUNC(EndMainMenuBar),
        REGISTER_LIB_FUNC(GetMouseDragDelta),
        REGISTER_LIB_FUNC(InputText),
        REGISTER_LIB_FUNC(IsItemActive),
        REGISTER_LIB_FUNC(IsItemClicked),
        REGISTER_LIB_FUNC(Image),
        REGISTER_LIB_FUNC(KeyCtrl),
        REGISTER_LIB_FUNC(KeySuper),
        REGISTER_LIB_FUNC(KeysDown),
        REGISTER_LIB_FUNC(KeysPressed),
        REGISTER_LIB_FUNC(MenuItem),
        REGISTER_LIB_FUNC(SameLine),
        REGISTER_LIB_FUNC(SetCursorPos),
        REGISTER_LIB_FUNC(Separator),
        REGISTER_LIB_FUNC(SetColumnWidth),
        REGISTER_LIB_FUNC(SetWindowSize),
        REGISTER_LIB_FUNC(Text),
        REGISTER_LIB_FUNC(TreeNode),
        REGISTER_LIB_FUNC(TreeNodeEx),
        REGISTER_LIB_FUNC(TreePop),

        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}
