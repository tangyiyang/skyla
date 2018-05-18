local skyla = require "skyla"
local stack = require "skyla.base.stack"
local lfs = require "lfs"
local scene_graph_editor = require "scene_graph_editor"
local record = require "record"


local editor = {
    root_dir_name = "",
    file_tree = {}
}

local function on_toolbar_node()
    print("on_toolbar_node")
end

local function on_toolbar_sprite()
    print("on_toolbar_sprite")
end

local function on_toolbar_scale9sprite()
    print("on_toolbar_scale9sprite")
end

local function on_toolbar_layercolor()
    print("on_toolbar_layercolor")
end

local function on_toolbar_label_ttf()
    print("on_toolbar_label_ttf")
end

local function on_toolbar_bmfont()
    print("on_toolbar_bmfont")
end

local function on_toolbar_menu()
    print("on_toolbar_menu")
end

local function on_toolbar_menu_item()
    print("on_toolbar_menu_item")
end

local function on_toolbar_paritcle()
    print("on_toolbar_paritcle")
end

local function on_toolbar_clipping_node()
    print("on_toolbar_clipping_node")
end

local avaliable_widgets_on_toolbar = {
    {name = "Node", func = on_toolbar_node},
    {name = "Sprite", func = on_toolbar_sprite},
    {name = "Scale9Sprite", func = on_toolbar_scale9sprite},
    {name = "LayerColor", func = on_toolbar_layercolor},
    {name = "LabelTTF", func = on_toolbar_label_ttf},
    {name = "LabelBMFont", func = on_toolbar_bmfont},
    {name = "Menu", func = on_toolbar_menu},
    {name = "MenuItem", func = on_toolbar_menu_item},
    {name = "Particle", func = on_toolbar_paritcle},
    {name = "ClippingNode", func = on_toolbar_clipping_node},
}

-- we must have these limitations when we have a LARGE folder.
local max_visit_depth = 128
local total_tree_items = 512
local visit_depth = 0
local tree_items = 0
local recursive_visit_path -- forward declartion for recursive function :)
recursive_visit_path = function(path, t)
    -- print("path = ", path)
    if visit_depth >= max_visit_depth or tree_items >= total_tree_items then
        return t
    end

    visit_depth = visit_depth + 1

    lfs.chdir(path)
    local pwd = lfs.currentdir()
    -- print("pwd = ", pwd)
    for file in lfs.dir(pwd) do
        if (string.byte(file) ~= string.byte(".")) then
            local f = path .. "/" .. file
            local attr = lfs.attributes(f)
            if attr and attr.mode == "directory" then
                t[file] = {}
                recursive_visit_path(f, t[file])
            else
                t[file] = "file"
            end
            tree_items = tree_items + 1
        end
    end

    return t
end

local function reload_file_tree(root_path)
    local saved_dir = lfs.currentdir()
    visit_depth = 0
    tree_items = 0
    editor.file_tree = recursive_visit_path(root_path, {})

    -- TODO: pattern matching?
    local s = root_path
    local i = #s

    -- /abc/ddd/ee -> get the ee
    while i >= 1 do
        if string.byte(s, i) == string.byte('/') then
            break
        end
        i = i - 1
    end
    editor.root_dir_name = string.sub(s, i+1, #s)
    lfs.chdir(saved_dir)
end

function editor.init()
    local settings = record.load_settings()
    print("the settings")
    print_r(settings)
    reload_file_tree(settings.work_dir)
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

local function pressed(key)
    local keys = imgui.KeysDown()

    local t = string.byte(key)
    for i = 1, #keys do
        if keys[i] == t then
            return true
        end
    end

    return false
end

local function response_to_shortcut()
    local super = imgui.KeySuper()
    local ctrl = imgui.KeyCtrl()

    if super and pressed("O") then
        print("pressed super+o")
    end

    if super and pressed("A") then
        print("pressed super+a")
    end

    if super and pressed("Z") then
        print("pressed super+z")
    end

    if super and pressed("C") then
        print("pressed super+c")
    end

    if super and pressed("V") then
        print("pressed super+v")
    end

    if super and pressed("S") then
        print("pressed super+s")
    end

    if super and pressed("X") then
        print("pressed super+x")
    end

    if super and pressed("Y") then
        print("pressed super+y")
    end
end

local function draw_menu()
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

local function draw_code_connection_tab()

end

local function draw_tool_bar()
    imgui.Begin("ToolBar", imgui.ImGuiWindowFlags_AlwaysUseWindowPadding)
    imgui.Columns(1)
    for _, w in ipairs(avaliable_widgets_on_toolbar) do
        if imgui.Button(w.name) then
            w.func()
        end
    end

    imgui.End()
end

local function draw_editor_scene()
    imgui.Begin("Scene")

    imgui.End()
end

local function editable(file_name)
    return string.match(file_name, ".json$") or string.match(file_name, ".png$")
end

local function proc_scene(file_name)
    print("call proc_scene, file_name = ", file_name)
end

local function proc_png(file_name)
    print("call proc_png, file_name = ", file_name)
end

local file_proc_funcs = {
    ["json"] = proc_scene,
    ["png"] = proc_png,
}

local function reload_scene(full_relative_path)
    local full_path = string.format("%s/%s", record.settings.work_dir, full_relative_path)
    scene_graph_editor.load(full_path)
end

local relative_path_from_file_tree
function relative_path_from_file_tree(file_tree, file_name, visit_path)
    for k, v in pairs(file_tree) do
        if type(v) == "table" then
            visit_path[#visit_path + 1] = k
            relative_path_from_file_tree(v, file_name, visit_path)
        else
            if v == file_name then
                return table.concat(visit_path, "/")
            end
        end
    end
end

local function on_click_file(file_tree, file_name, visit_stack)
    local relative_path = visit_stack:concat("/", 2)
    local full_relative_path = string.format("%s/%s", relative_path, file_name)

    local suffix = string.sub(file_name, string.find(file_name, "%.")+1, #file_name)
    local f = file_proc_funcs[suffix]
    if f then
        f(file_name)
    else
        error(string.format("unsupported file type, file_name = %s", file_name))
    end
end

local draw_file_tree
draw_file_tree = function(name, file_tree, visit_stack)
    visit_stack:push(name)
    if imgui.TreeNode(name) then
        for k, v in pairs(file_tree) do
            if type(v) == "table" then
                draw_file_tree(k, v, visit_stack)
            else
                if editable(k) then
                    if imgui.Button(k) then
                        on_click_file(file_tree, k, visit_stack)
                    end
                else
                    imgui.Text(k)
                end
            end
        end
        imgui.TreePop()
    end
    visit_stack:pop()
end

local function draw_file_system()
    imgui.Begin("Works")
    local work_dir = record.settings.work_dir
    if imgui.Button("Open") then
        local editor_core = require "editor.core"
        local dir = editor_core.pick_folder(work_dir)
        record.settings.work_dir = dir
        record.save()
        reload_file_tree(dir)
        print("the file tree is")
        print_r(editor.file_tree)
    end
    imgui.SameLine()
    imgui.InputText("path", record.settings.work_dir)
    imgui.Separator()

    -- draw the file tree and caculate the relative path while visiting
    local s = stack.new()
    draw_file_tree(editor.root_dir_name, editor.file_tree, s)
    imgui.End()
end

local function draw_scene_graph()
    scene_graph_editor.render()
end

function editor.update(dt)
    response_to_shortcut()
    draw_menu()
    draw_tool_bar()
    draw_file_system()
    draw_scene_graph()
end

function editor.destory()
    print("editor destroy")
end

return editor
