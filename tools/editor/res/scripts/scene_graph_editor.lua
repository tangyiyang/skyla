local cjson = require "cjson"

local scene_graph_editor = {
    scene_data = {},
}

local editing_node
local function render_property_editor()
    if editing_node then
        imgui.Begin("Property")

        -- Everybody needs "Code" and "Node"
        if imgui.CollapsingHeader("Code") then
            local ok, text
            ok, text = imgui.InputText("script", "")
            if ok then
                print("script = ", text)
                editing_node._opt.script = text
            end

            imgui.InputText("var", "")
        end

        if imgui.CollapsingHeader("Node") then
            local checked = imgui.Checkbox("Visible", editing_node:is_visible())
            editing_node:set_visible(checked)
            editing_node._opt.visible = checked
        end

        if editing_node.type == "Sprite" then

        end

        imgui.End()
    end
end


local scene_tree_flags =   imgui.ImGuiTreeNodeFlags_OpenOnArrow
                         | imgui.ImGuiTreeNodeFlags_OpenOnDoubleClick
local draw_node
function draw_node(node_data)
    if node_data and node_data.children then
        local opened = imgui.TreeNodeEx(node_data.display_name, scene_tree_flags)
        if opened then
            for i, child in ipairs(node_data.children) do
                draw_node(child)
            end
            imgui.TreePop()
        end
    else
        if imgui.Button(node_data.display_name) then
            print("click button ", node_data.display_name)
            editing_node = node_data._node
            skyla.dispatcher:emit({name = "on_editing_node_changed"}, editing_node)
        end
    end
end

local scene_root
local function on_node_clicked(node, ...)
    editing_node = node
    skyla.dispatcher:emit({name = "on_node_clicked"}, node, ...)
end

local function init_node_events(scene_node)
    visit_children_recursive(scene_node, function(node)
        node:on_touch(function(_, phase, x, y)
            on_node_clicked(node, phase, x, y)
        end)
    end)
end


local function load_scene(graph)
    local scene_loader = require("skyla.scene_loader")
    local game_scene_renderer = require "game_scene_renderer"

    require("skyla_util").add_search_path("example_project/")

    local node = scene_loader.load_from_data(graph)
    game_scene_renderer.reset_child(node)
    init_node_events(node)

    scene_root = node
end

function scene_graph_editor.init(file_full_path)
    -- we build a data structure to display the scene graph
    -- in order to not revisit the whole file every frame.
    print("file_full_path = ", file_full_path)
    local file = assert(io.open(file_full_path, "r"))
    local data = file:read("*a")
    local decoded = cjson.decode(data)
    assert(decoded and next(decoded) and decoded.data.setup,
            string.format("invalid file format: %s", file_full_path))
    file:close()

    scene_graph_editor.scene_data = decoded

    load_scene(decoded)

    skyla.dispatcher:on("super_s", function()
        local data = prettycjson(scene_graph_editor.scene_data)
        if data and #data > 0 then
            local file = assert(io.open(file_full_path, "w"))
            assert(file:write(data))
            file:flush()
            file:close()
        end
    end)
end

function scene_graph_editor.graph_to_json(graph)

end

function scene_graph_editor.render()
    if next(scene_graph_editor.scene_data) then
        imgui.Begin("SceneGraph")

        local setup_data = scene_graph_editor.scene_data.data.setup
        if setup_data and next(setup_data) then
            if imgui.TreeNode(setup_data.display_name) then
                local children = setup_data.children
                for i = 1, #children do
                    local child_data = children[i]
                    draw_node(child_data)
                end
                imgui.TreePop()
            end
        end

        render_property_editor()

        imgui.End()
    end
end

return scene_graph_editor
