/*
 * Complie time configuration for our engine
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_config_h
#define s2d_config_h

#ifndef S2D_MAX_SPRITE_VERTEX_BUFFER_SIZE 
#define S2D_MAX_SPRITE_VERTEX_BUFFER_SIZE (1024*16)
#endif

#ifndef S2D_MAX_LINE_VERTEX_BUFFER_SIZE
#define S2D_MAX_LINE_VERTEX_BUFFER_SIZE (1024*16)
#endif

#ifndef S2D_DEFAULT_LOGIC_WIDTH
#define S2D_DEFAULT_LOGIC_WIDTH (1280)
#endif

#ifndef S2D_DEFAULT_LOGIC_HEIGHT
#define S2D_DEFAULT_LOGIC_HEIGHT (720)
#endif

#ifndef S2D_INITIAL_TOUCHABLE_NODES
#define S2D_INITIAL_TOUCHABLE_NODES (64)
#endif

#ifndef S2D_ENABLE_LUA
#define S2D_ENABLE_LUA (1)

    #ifndef ENABLE_TYPE_CHECK
        #define ENABLE_TYPE_CHECK (1)
    #endif
#endif

#endif
