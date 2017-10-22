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
        print("before editor_info.visible = ", editor_info.visible)
        print("scene_graph_editor.property_editer_info = ", scene_graph_editor.property_editer_info.visible)
        editor_info.visible = imgui.Checkbox("Visible", editor_info.visible)
        print("after editor_info.visible = ", editor_info.visible)
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
local draw_scene_tree
function draw_scene_tree(tree)
    if tree.children then
        local opened = imgui.TreeNodeEx(tree.display_name, scene_tree_flags)
        if imgui.IsItemClicked() then
            on_open_property_editor(tree)
        end
        if opened then
            for i, child in ipairs(tree.children) do
                draw_scene_tree(child)
            end
            imgui.TreePop()
        end
    else
        if imgui.Button(tree.display_name) then
            on_open_property_editor(tree)
        end
    end
end

local build_scene_tree
function build_scene_tree(data, tree)
    for k,v in pairs(data) do
        if k == "node" or k == "sprite" then

            -- TODO: think twice if we do need copy these data.
            local node_data = data[k]
            tree.type = k
            tree.display_name = node_data.display_name or k
            tree.id = node_data.id
            tree.visible = node_data.visible
            print("node_data.visible = ", node_data.visible)

            local children = node_data.children
            if children and next(children) then
                local t = {}
                for i, child in ipairs(children) do
                    local child_data = {}
                    build_scene_tree(child, child_data)
                    t[#t+1] = child_data
                end
                tree["children"] = t
            end
        end
    end
    return tree
end

function scene_graph_editor.load(file_full_path)
    -- we build a data structure to display the scene graph
    -- in order to not revisit the whole file every frame.
    local file = io.open(file_full_path, "r")
    local data = file:read("*a")
    local decoded = cjson.decode(data)
    assert(decoded and next(decoded) and decoded.data.setup,
           string.format("invalid file format: %s", file_full_path))
    local tree = build_scene_tree(decoded.data.setup, {})
    scene_graph_editor.scene_tree.data = tree
    scene_graph_editor.scene_tree.name = decoded.data.name
end

function scene_graph_editor.render()
    imgui.Begin("SceneGraph")

    local scene_tree = scene_graph_editor.scene_tree
    if next(scene_tree) then
        if imgui.TreeNode(scene_tree.name) then
            draw_scene_tree(scene_tree.data)
            imgui.TreePop()
        end
    end

    render_property_editor()

    imgui.End()
end

return scene_graph_editor