-- TODO: refactor me!
local game_scene_renderer = {
}

function global_game_scene()
    if __scene_graph_canvas then
        assert(false, "game scene render canvas already loaded.")
        return
    end

    local visible_rect = require("skyla.context"):get_visible_rect()

    local root = go.ctx:get_root()
    local game_layer = go.node.new()
    game_layer:set_anchor(0, 0)

    root:add_child(game_layer)

    --[[
        FIXME: this is quite hacky, this function is called in
        C++ level, by using this global variable to store the data we
        have created. should we do all the stuff in Lua level?
    ]]
    _G["__scene_graph_canvas"] = game_layer
    return game_layer.__cobj
end


function game_scene_renderer.reset_child(child)
    local canvas = __scene_graph_canvas
    assert(canvas, "global game canvas not created.")
    canvas:remove_all_children()
    canvas:add_child(child)
end


return game_scene_renderer