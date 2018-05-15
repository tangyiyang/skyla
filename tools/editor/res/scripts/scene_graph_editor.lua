local cjson = require "cjson"

local scene_graph_editor = {
    scene_tree = {},
    property_editer_info = {}
}

local function render_property_editor()
    local editor_info = scene_graph_editor.property_editer_info
    imgui.Begin("Property")

    -- Everybody needs "Code" and "Node"
    if imgui.CollapsingHeader("Code") then
        imgui.InputText("Script", "main_scene.lua")
        imgui.InputText("Var", "_root")
    end

    if imgui.CollapsingHeader("Node") then
        editor_info.visible = imgui.Checkbox("Visible", editor_info.visible)
    end

    if editor_info.type == "Sprite" then

    end

    imgui.End()
end

local function on_open_property_editor(node_tree_info)
    print("click " .. node_tree_info.display_name)
    scene_graph_editor.property_editer_info = node_tree_info
    print_r(node_tree_info)
end

local scene_tree_flags =   imgui.ImGuiTreeNodeFlags_OpenOnArrow
                         | imgui.ImGuiTreeNodeFlags_OpenOnDoubleClick
local draw_node
function draw_node(node_data)
    if node_data and node_data.children then
        local opened = imgui.TreeNodeEx(node_data.display_name, scene_tree_flags)
        if imgui.IsItemClicked() then
            on_open_property_editor(node_data)
        end
        if opened then
            for i, child in ipairs(node_data.children) do
                draw_node(child)
            end
            imgui.TreePop()
        end
    else
        if imgui.Button(node_data.display_name) then
            on_open_property_editor(node_data)
        end
    end
end


function scene_graph_editor.load(file_full_path)
    -- we build a data structure to display the scene graph
    -- in order to not revisit the whole file every frame.
    print("file_full_path = ", file_full_path)
    local file = assert(io.open(file_full_path, "r"))
    local data = file:read("*a")
    local decoded = cjson.decode(data)
    assert(decoded and next(decoded) and decoded.data.setup,
           string.format("invalid file format: %s", file_full_path))
    scene_graph_editor.scene_data = decoded

    print("the scene data is")
    print_r(decoded)


end

function scene_graph_editor.render()
    if scene_graph_editor.scene_data then
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
