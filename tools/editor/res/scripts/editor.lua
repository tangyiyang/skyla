local core = require "editor.core"
local editor = {}

-- local imgui = require "imgui"

function editor.start()
    print("editor start")
    core.inject {
        on_init = editor.init,
        on_update = editor.update,
        on_destory = editor.destroy,
    }
end

function editor.init()
    print("editor init")
end

local function on_open_file_menu()
    if (imgui.MenuItem("New")) then

    end

    if (imgui.MenuItem("Open", "Ctrl+O")) then

    end

    if (imgui.BeginMenu("Open Recent")) then
        imgui.MenuItem("fish_hat.c");
        imgui.MenuItem("fish_hat.inl");
        imgui.MenuItem("fish_hat.h");
        if (imgui.BeginMenu("More..")) then
            imgui.MenuItem("Hello");
            imgui.MenuItem("Sailor");
            if (imgui.BeginMenu("Recurse..")) then

                imgui.EndMenu();

            end
            imgui.EndMenu();
        end
        imgui.EndMenu();
    end
    if (imgui.MenuItem("Save", "Ctrl+S")) then end
    if (imgui.MenuItem("Save As..")) then end
    imgui.Separator();

    if (imgui.BeginMenu("Disabled", false)) then
        IM_ASSERT(0);
    end
    if (imgui.MenuItem("Checked", NULL, true)) then end
    if (imgui.MenuItem("Quit", "Alt+F4")) then end

end

function editor.update(dt)
    print("editor update")
    if (imgui.BeginMainMenuBar()) then
        if (imgui.BeginMenu("File")) then

            on_open_file_menu()

            imgui.EndMenu();
        end

        if (imgui.BeginMenu("Edit")) then
            if (imgui.MenuItem("Undo", "CTRL+Z")) then
            end

            if (imgui.MenuItem("Redo", "CTRL+Y", false, false)) then
            end

            imgui.Separator()
            if (imgui.MenuItem("Cut", "CTRL+X")) then
            end
            if (imgui.MenuItem("Copy", "CTRL+C")) then
            end
            if (imgui.MenuItem("Paste", "CTRL+V")) then
            end

            imgui.EndMenu()
        end
        imgui.EndMainMenuBar()
    end
end

function editor.destory()
    print("editor destroy")
end

return editor
