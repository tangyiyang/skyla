#include "s2d_lua_context.h"
#include "s2d_util.h"
#include "s2d_sprite.h"
#include "s2d_context.h"

#if (S2D_ENABLE_LUA == 1)

#ifdef __cplusplus
extern "C" {
extern int luaopen_cjson(lua_State* L);
}
#endif

NS_S2D

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
    lua_pop(L, -1);
    return 0;
}


static int lseal2d_context_get_root(lua_State* L)
{
    context* C = context::C();
    lua_pushlightuserdata(L, C->get_root());
    return 1;
}

static int lseal2d_context_get_visible_rect(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    context* C = (context*)lua_touserdata(L, -1);

    rect* r = &(C->_visible_rect);
    lua_newtable(L);
    lua_pushnumber(L, r->origin.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, r->origin.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, r->size.width);
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, r->size.height);
    lua_setfield(L, -2, "height");

    return 1;
}

static int lseal2d_context(lua_State* L)
{
    static context* C = context::C();
    lua_pushlightuserdata(L, C);
    return 1;
}

static int lseal2d_node_add_child(lua_State* L)
{
    node* n = (node*)lua_touserdata(L, 1);
    node* child = (node*)lua_touserdata(L, 2);

    n->add_child(child);
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

static int lseal2d_node_new(lua_State* L)
{
    node* n = new node();
    n->init();
    lua_pushlightuserdata(L, n);
    return 1;
}
static int luaopen_seal2d_node(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",         lseal2d_node_new },
        { "add_child", lseal2d_node_add_child },
        { "set_visible", lseal2d_node_set_visible},
        { "set_pos", lseal2d_node_set_pos },
        { "set_anchor", lseal2d_node_set_anchor },
        { "set_rotation", lseal2d_node_set_rotation },
        { "set_scale", lseal2d_node_set_scale },
        { "set_size", lseal2d_node_set_size },
        { "get_size", lseal2d_node_get_size },
        { "on_event", lseal2d_node_on_event },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}

static int lseal2d_sprite_new(lua_State* L)
{
    int n = lua_gettop(L);

    sprite* s = nullptr;
    if (n == 1) {
        s = new sprite();
        char* file_name = (char*)luaL_checkstring(L, 1);
        if (file_name[0] == '#') {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->init(f);
        } else {
            s->init(file_name);
        }
    } else if (n == 0) {
        s = new sprite();
        s->init();
    } else {
        luaL_error(L, "invalid arguments to seal2d.new_sprite");
        return 0;
    }

    lua_pushlightuserdata(L, s);
    return 1;
}

static int lseal2d_sprite_set_color(lua_State* L)
{
    sprite* s = (sprite*)lua_touserdata(L, 1);
    lua_Integer color = luaL_checkinteger(L, 2);
    s->set_color((uint32_t)color);
    return 0;
}

static int lseal2d_sprite_set_texture(lua_State* L)
{
    int n = lua_gettop(L);
    if (n < 2) {
        luaL_error(L, "invalid number of arguments passed to sprite_set_texture"
                   "expected more than 2, but got %d", n);
        return 0;
    }

    sprite* s = (sprite*)lua_touserdata(L, 1);
    if (n == 2) {
        char* file_name = (char*)luaL_checkstring(L, 2);
        if (file_name[0] == '#') {
            sprite_frame* f = context::C()->_sprite_frame_cache->get(file_name+1);
            s->set_texture(f);
        } else {
            s->set_texture(file_name);
        }
    }

    return 0;
}

static int lseal2d_sprite_set_blend_mode(lua_State* L)
{
    int n = lua_gettop(L);
    if (n != 2) {
        luaL_error(L, "invalid number of arguments passed to sprite_set_blend_mode"
                   "expected more than 2, but got %d", n);
        return 0;
    }
    sprite* s = (sprite*)lua_touserdata(L, 1);
    lua_Integer mode = luaL_checkinteger(L, 2);

    S2D_ASSERT(mode >= 0 && mode < BLEND_MODE_MAX);
    s->set_blend_mode((enum blend_mode)mode);

    return 0;
}

int luaopen_seal2d_sprite(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lseal2d_sprite_new },
        { "set_texture",    lseal2d_sprite_set_texture },
        { "set_color",      lseal2d_sprite_set_color},
        { "set_blend_mode", lseal2d_sprite_set_blend_mode},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}

int lseal2d_new_bmfont(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        const char* text = luaL_checkstring(L, 1);
        const char* font_atlas_path = luaL_checkstring(L, 2);

        bmfont* n = new bmfont();
        n->init(text, font_atlas_path);

        lua_pushlightuserdata(L, n);
        return 1;
    }

    luaL_error(L, "invalid number of arguments in seal2d_new_bmfont"
               " expected 2, but got %d", n);
    return 0;
}

int lseal2d_bmfont_set_text(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2){
        bmfont* b = (bmfont*)lua_touserdata(L, 1);
        const char* text = luaL_checkstring(L, 2);
        b->set_text(text);
        return 1;
    }

    luaL_error(L, "invalid number of arguments in seal2d_new_bmfont"
               " expected 2, but got %d", n);
    return 0;
}

int luaopen_seal2d_bmfont(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",            lseal2d_new_bmfont },
        { "set_text",       lseal2d_bmfont_set_text   },
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

static int lua_seal2d_timer_new(lua_State* L)
{
    int n = lua_gettop(L);
    if (n == 2) {
        timer* t = new timer();

        float interval = luaL_checknumber(L, 1);

        lua_getfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_TIMER_TABLE);
        lua_pushlightuserdata(L, t);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);

        t->init(interval, [=](void*){
            lua_getfield(L, LUA_REGISTRYINDEX, SEAL2D_USER_TIMER_TABLE); /* event_table */
            lua_pushlightuserdata(L, t); /* event_table node */
            lua_gettable(L, -2);    /* event_table node function */

            lua_context::call_lua(L, 0, 0);
            lua_pop(L, -1);
        }, nullptr);

        lua_pop(L, -1);

        lua_pushlightuserdata(L, t);
        return 1;
    }
    luaL_error(L, "error args passed to lua_seal2d_timer_new"
                  "expected 3, but got %d", n);
    return 0;
}

static int lua_seal2d_timer_start(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    timer* C = (timer*)lua_touserdata(L, -1);
    C->start();
    return 0;
}

static int lua_seal2d_timer_stop(lua_State* L)
{
    lua_getfield(L, 1, "__cobj");
    timer* C = (timer*)lua_touserdata(L, -1);
    C->stop();
    return 0;
}

static int luaopen_seal2d_timer(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "new",  lua_seal2d_timer_new },
        { "start", lua_seal2d_timer_start },
        { "stop", lua_seal2d_timer_stop },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}


static int luaopen_seal2d_context(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif

    luaL_Reg lib[] = {
        { "get_root",         lseal2d_context_get_root },
        { "get_visible_rect", lseal2d_context_get_visible_rect },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

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

void lua_context::register_lua_extensions(lua_State* L)
{
    luaL_Reg lua_modules[] = {
        { "cjson",          luaopen_cjson       },
        { "seal2d",         luaopen_seal2d      },
        { "seal2d_node",    luaopen_seal2d_node },
        { "seal2d_sprite",  luaopen_seal2d_sprite },
        { "seal2d_bmfont",  luaopen_seal2d_bmfont },
        { "seal2d_context", luaopen_seal2d_context},
        { "seal2d_util",    luaopen_seal2d_util },
        { "seal2d_timer",   luaopen_seal2d_timer },

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
    lua_pushcfunction(L, traceback);

    _lua_state = L;
}

void lua_context::on_start(context* ctx, const char* script_path)
{
    lua_State* L = _lua_state;
    int r = luaL_loadfile(L, script_path);
    if (r != LUA_OK) {
        LOGE("error load lua file\n");
        return;
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

NS_S2D_END

#endif
