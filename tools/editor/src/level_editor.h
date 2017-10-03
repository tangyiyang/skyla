#ifndef level_editor_h
#define level_editor_h

#include "level_editor_common.h"
#include "cmds.h"

NS_GAME_TOOL

class level_editor {
public:
    level_editor();

public:
    void init_scene();
    void shutdown_scene();

    void draw();

private:
    void show_menu_bar();
    void show_tool_bar();
    void show_menu_file();

    void show_editor_scene();

private:
    bool _tool_bar_visible;
    std::queue<cmd*> _cmd_stack;
};

NS_GAME_TOOL_END
#endif /* level_editor_h */
