local node_editor = {}

-- TODO: get them from binding api.
local cursor_start_x = 8
local cursor_start_y = 27

local window_width = 1280
local window_height = 720

local game_to_window_scale = 2
local editing_node

local function node_pos_to_window(node)
    -- node to world.
    local world_x, world_y = node:local_to_world()

    local tx = world_x / game_to_window_scale
    local ty = (window_height - world_y) / game_to_window_scale

    local x = tx + cursor_start_x
    local y = ty + cursor_start_y

    return x, y
end

local function node_size_to_window(node)
    local w, h = node:get_size()
    return w / game_to_window_scale, h / game_to_window_scale
end

function node_editor.reset()
    editing_node = nil
end

function node_editor.init()
    skyla.dispatcher:on("on_node_clicked", function(_, node, phase, x, y)
        if phase == TOUCH_BEGAN then
            editing_node = node
            node_pos_to_window(node)
            print_r(node._opt)
        end
    end)

    local root = go.ctx:get_root()
    root:on_touch(function(_, phase, _, _)
        if phase == TOUCH_ENDED then
            editing_node = nil
        end
    end)

    local btn_texture = skyla.texture.new("res/images/select_outline_green.png")
    local id = btn_texture:get_texture_opengl_id()
    skyla.dispatcher:on("on_render_game_scene", function()

        if editing_node then
            --translate the node position to game_scene window and draw a rect
            local x, y = node_pos_to_window(editing_node)
            local w, h = node_size_to_window(editing_node)

            imgui.SetCursorPos(x, y-h)
            imgui.Image(id, w, h)

            local x, y = imgui.GetMouseDragDelta()
            if x ~= 0 and y ~= 0 then
                y = -y -- imgui coord is upsidedown
                x = x * game_to_window_scale
                y = y * game_to_window_scale
                local new_x = editing_node.__ox + x
                local new_y = editing_node.__oy + y

                -- set the node position and save to the opt
                editing_node:set_pos(new_x, new_y)
                editing_node._opt.position.x = new_x
                editing_node._opt.position.y = new_y
            else
                editing_node.__ox, editing_node.__oy = editing_node:get_pos()
            end
        end
    end)
end

return node_editor