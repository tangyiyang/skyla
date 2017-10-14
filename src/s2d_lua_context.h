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

#ifndef s2d_lua_conext_h
#define s2d_lua_conext_h

#include "s2d_common.h"

#ifdef S2D_ENABLE_LUA

typedef int LUA_FUNC_HANDLER;

#ifdef __cplusplus
extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
}
#endif

#define CONTEXT_START "CONTEXT_START"
#define CONTEXT_UPDATE "CONTEXT_UPDATE"
#define CONTEXT_DESTROY "CONTEXT_DESTROY"

NS_S2D

class context;
class lua_context {
public:
    static void stackDump (lua_State* L);
    static int call_lua(lua_State* L, int n, int r);

public:
    void init();
    void on_start(context* ctx, const char* script_path);
    void on_update(uint32_t dt);
    void on_destroy();

private:
    void register_lua_extensions();
    
public:
    lua_State* _lua_state;
    LUA_FUNC_HANDLER _event_handler;
};

NS_S2D_END

#endif /* end of S2D_ENABLE_LUA */

#endif
