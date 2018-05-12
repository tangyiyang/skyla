/*
 * Complie time configuration for our engine
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_config_h
#define skyla_config_h

#ifndef SKYLA_MAX_VERTEX_PER_DRAW_CALL
#define SKYLA_MAX_VERTEX_PER_DRAW_CALL (1024*16)
#endif

#ifndef SKYLA_MAX_LINE_VERTEX_BUFFER_SIZE
#define SKYLA_MAX_LINE_VERTEX_BUFFER_SIZE (1024*16)
#endif

#ifndef SKYLA_DEFAULT_LOGIC_WIDTH
#define SKYLA_DEFAULT_LOGIC_WIDTH (1280)
#endif

#ifndef SKYLA_DEFAULT_LOGIC_HEIGHT
#define SKYLA_DEFAULT_LOGIC_HEIGHT (720)
#endif

#ifndef SKYLA_INITIAL_TOUCHABLE_NODES
#define SKYLA_INITIAL_TOUCHABLE_NODES (64)
#endif

#ifndef SKYLA_ENABLE_LUA
#define SKYLA_ENABLE_LUA (1)

    #ifndef ENABLE_TYPE_CHECK
        #define ENABLE_TYPE_CHECK (1)
    #endif
#endif

#endif
