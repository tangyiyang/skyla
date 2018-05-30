local node_editor = {}

-- TODO: get them from binding api.
local cursor_start_x = 8
local cursor_start_y = 27

local window_width = 1280
local window_height = 720

local function node_pos_to_window(node)
    -- node to world.
    local world_x, world_y = node:local_to_world()

    printf("world_x, world_y = %.2f, %.2f", world_x, world_y)
    local tx = world_x / 2
    local ty = (window_height - world_y) / 2

    printf("tx, ty = %.2f, %.2f", tx, ty)

    local x = tx + cursor_start_x
    local y = ty + cursor_start_y

    printf("x, y = %.2f, %.2f", x, y)
    return x, y
end

local function node_size_to_window(node)
    local w, h = node:get_size()
    return w / 2, h / 2
end

function node_editor.init()
    print("call node_editor.init")

    local editing_node
    skyla.dispatcher:on("on_node_clicked", function(_, node, phase, x, y)
        printf("node = %d, phase = %d, x, y = %.2f, %.2f", node:get_id(), phase, x, y)

        editing_node = node

        node_pos_to_window(node)
    end)


    local btn_texture = skyla.texture.new("res/images/select_outline_green.png")
    local id = btn_texture:get_texture_opengl_id()
    print("id = ", id)
    skyla.dispatcher:on("on_render_game_scene", function()

        if editing_node then
            --translate the node position to game_scene window and draw a rect
            local x, y = node_pos_to_window(editing_node)
            local w, h = node_size_to_window(editing_node)

            imgui.SetCursorPos(x, y-h)
            imgui.Image(id, w, h)
        end

    end)
end

return node_editor