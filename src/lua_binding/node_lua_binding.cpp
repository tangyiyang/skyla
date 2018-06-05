#include "node_lua_binding.h"
#include "game_objects/node.h"

#if (SKYLA_ENABLE_LUA == 1)

NS_SKYLA

static int lskyla_node_add_child(lua_State* L)
{
    node* n = (node*)lua_touserdata(L, 1);
    node* child = (node*)lua_touserdata(L, 2);
    lua_Integer zorder = luaL_checkinteger(L, 3);
    n->add_child(child, zorder);
    return 0;
}

static int lskyla_node_get_children(lua_State* L)
{
    node* n = (node*)lua_touserdata(L, 1);
    const std::vector<node*>& children = n->get_children();
    lua_pop(L, -1);
    lua_newtable(L);
    std::vector<node*>::const_iterator it = children.begin();
    int i = 1;
    for (; it != children.end(); ++it)
    {
        lua_pushinteger(L, i++);
        lua_pushlightuserdata(L, *it);
        lua_settable(L, -3);
    }
    return 1;
}

static int lskyla_node_local_to_world(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    vec2 pos = n->local_to_world();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

static int lskyla_node_remove_all_children(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    n->remove_all_children();
    return 0;
}

static int lskyla_node_remove_from_parent(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    n->remove_from_parent();
    return 0;
}

static int lskyla_node_set_visible(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    bool visible = lua_toboolean(L, 2);
    n->set_visible(visible);
    return 0;
}

static int lskyla_node_set_size(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number width = luaL_checknumber(L, 2);
    lua_Number height = luaL_checknumber(L, 3);
    n->set_size(width, height);
    return 0;
}

static int lskyla_nod_is_visible(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_pushboolean(L, n->is_visible());
    return 1;
}

static int lskyla_node_get_size(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    const size& s = n->get_size();
    lua_pushnumber(L, s.width);
    lua_pushnumber(L, s.height);

    return 2;
}

static int lskyla_node_get_scale(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    const vec2& s = n->get_scale();
    lua_pushnumber(L, s.x);
    lua_pushnumber(L, s.y);

    return 2;
}

static int lskyla_node_get_pos(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    const vec2& p = n->get_pos();
    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

static int lskyla_node_get_bounding_box(lua_State* L)
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

static int lskyla_node_set_pos(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number x = luaL_checknumber(L, 2);
    lua_Number y = luaL_checknumber(L, 3);
    n->set_pos(x, y);
    return 0;
}

static int lskyla_node_set_anchor(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number ax = luaL_checknumber(L, 2);
    lua_Number ay = luaL_checknumber(L, 3);
    n->set_anchor(ax, ay);
    return 0;
}

static int lskyla_node_set_rotation(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_Number r = luaL_checknumber(L, 2);
    n->set_rotation(r);
    return 0;
}

static int lskyla_node_set_scale(lua_State* L)
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

static int lskyla_node_on_touch(lua_State* L)
{
    int n_args = lua_gettop(L);
    if (n_args == 2) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, LUA_REGISTRYINDEX, SKYLA_TOUCH_FUNC_TABLE);
        lua_pushlightuserdata(L, n);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);

        n->set_touch_callback([=](node*, touch_event* e) {
            lua_getfield(L, LUA_REGISTRYINDEX, SKYLA_TOUCH_FUNC_TABLE); /* event_table */
            lua_pushlightuserdata(L, n); /* event_table node */
            lua_gettable(L, -2);    /* event_table node function */
            lua_pushinteger(L, e->_id); /* event_table node function "skyla.event.touch" id */
            lua_pushinteger(L, e->_phase); /* event_table node function "skyla.event.touch" id phase */
            lua_pushnumber(L, e->_pos.x); /* event_table node function "skyla.event.touch" id phase x */
            lua_pushnumber(L, e->_pos.y); /* event_table node function "skyla.event.touch" id phase x y*/

            lua_context::call_lua(L, 4, 0);
            lua_pop(L, -1);
        });
        return 0;
    }

    luaL_error(L, "invalid number of arguments passed to skyla_node_on_touch"
               "expected 2, but got %d", n_args);
    return 0;
}

static int lsea2d_node_on_update(lua_State* L)
{
    int n_args = lua_gettop(L);
    if (n_args == 2) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, LUA_REGISTRYINDEX, SKYLA_UPDATE_FUNC_TABLE);
        lua_pushlightuserdata(L, n);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);

        n->set_update_callback([=](node*, float dt) {
            lua_getfield(L, LUA_REGISTRYINDEX, SKYLA_UPDATE_FUNC_TABLE); /* event_table */
            lua_pushlightuserdata(L, n); /* event_table node */
            lua_gettable(L, -2);    /* event_table node function */
            lua_pushnumber(L, dt); /* event_table node function "skyla.event.touch" id */

            lua_context::call_lua(L, 1, 0);
            lua_pop(L, -1);
        });
        return 0;
    }

    luaL_error(L, "invalid number of arguments passed to skyla_node_on_touch"
               "expected 2, but got %d", n_args);
    return 0;

}

static int lskyla_node_run_action(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);

        action* a = (action*)lua_touserdata(L, 2);
        n->run_action(a);

        return 0;
    }

    luaL_error(L, "invalid number of arguments passed to lskyla_node_run_action"
               "expected 2, but got %d", n);
    return 0;
}

static int lskyla_node_stop_all_actions(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 1) {
        lua_getfield(L, 1, "__cobj");
        node* n = (node*)lua_touserdata(L, -1);
        n->stop_all_actions();
        return 0;
    }
    luaL_error(L, "invalid number of arguments passed to lskyla_node_stop_all_actions"
               "expected 1, but got %d", n);
    return 0;
}

static int lskyla_node_get_id(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    node* n = (node*)lua_touserdata(L, -1);
    lua_pushinteger(L, n->get_id());

    return 1;
}

static int lskyla_node_new(lua_State* L)
{
    node* n = new node();
    n->init();
    lua_pushlightuserdata(L, n);
    return 1;
}

int luaopen_skyla_node(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",         lskyla_node_new },
        { "get_id",    lskyla_node_get_id },
        { "add_child", lskyla_node_add_child },
        { "get_children", lskyla_node_get_children },
        { "local_to_world", lskyla_node_local_to_world },
        { "remove_from_parent", lskyla_node_remove_from_parent },
        { "remove_all_children", lskyla_node_remove_all_children },
        { "set_visible", lskyla_node_set_visible},
        { "set_pos", lskyla_node_set_pos },
        { "set_anchor", lskyla_node_set_anchor },
        { "set_rotation", lskyla_node_set_rotation },
        { "set_scale", lskyla_node_set_scale },
        { "set_size", lskyla_node_set_size },
        { "is_visible", lskyla_nod_is_visible },
        { "get_size", lskyla_node_get_size },
        { "get_scale", lskyla_node_get_scale },
        { "get_pos", lskyla_node_get_pos },
        { "get_bounding_box", lskyla_node_get_bounding_box },
        { "on_touch", lskyla_node_on_touch },
        { "on_update", lsea2d_node_on_update },
        { "run_action", lskyla_node_run_action },
        { "stop_all_actions", lskyla_node_stop_all_actions },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}
NS_SKYLA_END

#endif


