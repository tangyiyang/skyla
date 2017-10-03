#ifndef cmds_h
#define cmds_h

#include "level_editor_common.h"

NS_GAME_TOOL

class cmd {
public:
    virtual void exec() = 0;   // do
    virtual void un_exec() = 0; // undo
};

class cmd_add_sprite : public cmd {
public:
    void exec();
    void un_exec();
};

NS_GAME_TOOL_END

#endif /* cmds_h */
