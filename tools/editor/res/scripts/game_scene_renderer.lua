local canvas

function global_game_scene()
    if canvas then
        assert(false, "global_game_scene initlize func should be called only once.")
        return
    end

    local visible_rect = require("skyla.context"):get_visible_rect()

    local root = go.ctx:get_root()
    local game_layer = go.node.new()
    game_layer:set_anchor(0, 0)

    root:add_child(game_layer)

    canvas = game_layer

    game_scene_renderer.game_layer = game_layer
    return game_layer.__cobj
end


local game_scene_renderer = { }
function game_scene_renderer.reload_child(child)
    assert(canvas, "global game canvas not created.")
    canvas:remove_all_children()
    canvas:add_child(child)
end


return game_scene_renderer