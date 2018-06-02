local game_scene_renderer = {}

--[[
        FIXME: this is quite hacky, this function is called in
        C++ level, by using this global variable to store the data we
        have created. should we do all the stuff in Lua level?
    ]]
function global_game_scene()
    if __scene_graph_canvas then
        assert(false, "game scene render canvas already loaded.")
        return
    end
    require("skyla_util").add_search_path("res/")

    local visible_rect = require("skyla.context"):get_visible_rect()

    local root = go.ctx:get_root()
    local game_layer = go.node.new()
    game_layer:set_anchor(0, 0)
    root:add_child(game_layer)

    -- add the canvas area

    local function draw_background_grids()
        local panel = go.panel.new(0, 0, visible_rect.width, visible_rect.height)
        -- TODO: add more reasonable grid caculation algorithem.
        local grid_width, grid_height = 20, 20 -- the image size is 20, 20
        local w = math.ceil(visible_rect.width / grid_width)
        local h = math.ceil(visible_rect.height / grid_height)

        for i = 1, w do
            for j = 1, h do
                local grid = go.sprite.new("images/grid.png")
                grid:set_pos((i-1) * grid_width, (j-1) * grid_height)
                grid:set_anchor(0, 0)
                panel:add_child(grid)
            end
        end

        game_layer:add_child(panel)
    end

    draw_background_grids()

    _G["__scene_graph_canvas"] = game_layer
    return game_layer.__cobj
end

function on_render_game_scene()
    skyla.dispatcher:emit("on_render_game_scene")
end

function game_scene_renderer.reset_child(child)
    local canvas = __scene_graph_canvas
    assert(canvas, "global game canvas not created.")
    if canvas.__user_child then
        require("node_editor").reset()
        canvas.__user_child:remove_from_parent()
    end
    canvas.__user_child = child
    canvas:add_child(child)
end

return game_scene_renderer