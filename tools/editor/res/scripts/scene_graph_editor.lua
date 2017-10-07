local cjson = require "cjson"

local scene_graph_editor = {
    scene_tree = {}
}

local draw_scene_tree
function draw_scene_tree(tree)
    if tree.children then
        if imgui.TreeNode(tree.display_name) then
            for i, child in ipairs(tree.children) do
                draw_scene_tree(child)
            end
            imgui.TreePop()
        end
    else
        imgui.Text(tree.display_name)
    end
end

local build_scene_tree
function build_scene_tree(data, tree)
    for k,v in pairs(data) do
        if k == "node" or k == "sprite" then
            -- parse the display_name first
            -- TODO: we may need more
            local node_data = data[k]

            tree.display_name = node_data.display_name or k
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

    imgui.End()
end

return scene_graph_editor