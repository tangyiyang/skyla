local core = require "editor.core"
local imgui = require "imgui"
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

local checked = false

local function on_open_file_menu()
    if (imgui.MenuItem("New")) then

    end

    if (imgui.MenuItem("Open", "Ctrl+O")) then
        print("ctrl + o");
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
    local t
    t, checked = imgui.MenuItem("Checked", nil, checked)
    if t then
        print("checked = ", checked)
    end

    if (imgui.MenuItem("Quit", "Alt+F4")) then end

end

local function response_to_shortcut()
    -- local io = imgui.GetIO();
    -- local n = io.KeysDown
    -- print("n = ", n);
--     for (i  i < IM_ARRAYSIZE(io.KeysDown); i++) {
-- //            if (io.KeysDownDuration[i] >= 0.0f) {
-- //                ImGui::SameLine();
-- //                ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]);
-- //            }
-- //        }
end


function editor.update(dt)
    -- response_to_shortcut()

    if (imgui.BeginMainMenuBar()) then
        if (imgui.BeginMenu("File")) then

            on_open_file_menu()

            imgui.EndMenu();
        end

        if (imgui.BeginMenu("Edit")) then
            if (imgui.MenuItem("Undo", "CTRL+Z")) then
                print("ctrl + z");
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
