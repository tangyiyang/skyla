#include "s2d_node_lua_binding.h"
#include "s2d_node.h"

#if (S2D_ENABLE_LUA == 1)

NS_S2D

static int lseal2d_node_add_child(lua_State* L)
{
    node* n = (node*)lua_touserdata(L, 1);
    node* child = (node*)lua_touserdata(L, 2);

    n->add_child(child);
    return 0;
}

static int lseal2d_node_remove_all_children(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    n->remove_all_children();
    return 0;
}

static int lseal2d_node_set_visible(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    bool visible = lua_toboolean(L, 2);
    n->set_visible(visible);
    return 0;
}

static int lseal2d_node_set_size(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number width = luaL_checknumber(L, 2);
    lua_Number height = luaL_checknumber(L, 3);
    n->set_size(width, height);
    return 0;
}

static int lseal2d_node_get_size(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    const size& s = n->get_size();
    lua_pushnumber(L, s.width);
    lua_pushnumber(L, s.height);

    return 2;
}

static int lseal2d_node_get_pos(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    const vec2& p = n->get_pos();
    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

static int lseal2d_node_get_bounding_box(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    const rect& r = n->get_bounding_box();
    lua_pushnumber(L, r.origin.x);
    lua_pushnumber(L, r.origin.y);
    lua_pushnumber(L, r.size.width);
    lua_pushnumber(L, r.size.height);
    return 4;
}

static int lseal2d_node_set_pos(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number x = luaL_checknumber(L, 2);
    lua_Number y = luaL_checknumber(L, 3);
    n->set_pos(x, y);
    return 0;
}

static int lseal2d_node_set_anchor(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number ax = luaL_checknumber(L, 2);
    lua_Number ay = luaL_checknumber(L, 3);
    n->set_anchor(ax, ay);
    return 0;
}

static int lseal2d_node_set_rotation(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number r = luaL_checknumber(L, 2);
    n->set_rotation(r);
    return 0;
}

static int lseal2d_node_set_scale(lua_State* L)
{
    int n_args = lua_gettop(L);
    if (n_args == 2) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        lua_Number scale = luaL_checknumber(L, 2);
        n->set_scale(scale);
    } else if (n_args == 3) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        lua_Number x = luaL_checknumber(L, 2);
        lua_Number y = luaL_checknumber(L, 3);
        n->set_scale(x, y);
    } else {
        luaL_error(L, "node.set_scale has got wrong number of arguments, expected 2 or 3, but got %d", n_args);
    }

    return 0;
}

static int lseal2d_node_on_event(lua_State* L)
{
    int n_args = lua_gettop(L);
    if (n_args == 2) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_FUNC_TABLE);
        lua_pushlightuserdata(L, n);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);

        n->set_touch_callback([=](void*, touch_event* e){

            lua_getfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_FUNC_TABLE); /* event_table */
            lua_pushlightuserdata(L, n); /* event_table node */
            lua_gettable(L, -2);    /* event_table node function */
            lua_pushstring(L, SEAL2D_EVENT_TOUCH); /* event_table node function "seal2d.event.touch" */
            lua_pushinteger(L, e->_id); /* event_table node function "seal2d.event.touch" id */
            lua_pushinteger(L, e->_phase); /* event_table node function "seal2d.event.touch" id phase */
            lua_pushnumber(L, e->_pos.x); /* event_table node function "seal2d.event.touch" id phase x */
            lua_pushnumber(L, e->_pos.y); /* event_table node function "seal2d.event.touch" id phase x y*/

            lua_context::call_lua(L, 5, 0);
            lua_pop(L, -1);
        });
        return 0;
    }

    luaL_error(L, "invalid number of arguments passed to seal2d_node_on_touch"
               "expected 2, but got %d", n_args);
    return 0;
}

static int lseal2d_node_run_action(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);

        action* a = (action*)lua_touserdata(L, 2);
        n->run_action(a);

        return 0;
    }

    luaL_error(L, "invalid number of arguments passed to lseal2d_node_run_action"
               "expected 2, but got %d", n);
    return 0;
}

static int lseal2d_node_stop_all_actions(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 1) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        n->stop_all_actions();
        return 0;
    }
    luaL_error(L, "invalid number of arguments passed to lseal2d_node_stop_all_actions"
               "expected 1, but got %d", n);
    return 0;
}

static int lseal2d_node_new(lua_State* L)
{
    node* n = new node();
    n->init();
    lua_pushlightuserdata(L, n);
    return 1;
}

int luaopen_seal2d_node(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",         lseal2d_node_new },
        { "add_child", lseal2d_node_add_child },
        { "remove_all_children", lseal2d_node_remove_all_children },
        { "set_visible", lseal2d_node_set_visible},
        { "set_pos", lseal2d_node_set_pos },
        { "set_anchor", lseal2d_node_set_anchor },
        { "set_rotation", lseal2d_node_set_rotation },
        { "set_scale", lseal2d_node_set_scale },
        { "set_size", lseal2d_node_set_size },
        { "get_size", lseal2d_node_get_size },
        { "get_pos", lseal2d_node_get_pos },
        { "get_bounding_box", lseal2d_node_get_bounding_box },
        { "on_event", lseal2d_node_on_event },
        { "run_action", lseal2d_node_run_action },
        { "stop_all_actions", lseal2d_node_stop_all_actions },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

#endif

NS_S2D_END
