function global_game_scene()
    local visible_rect = require("skyla.context"):get_visible_rect()

    local root = go.ctx:get_root()
    local game_layer = go.node.new()
    game_layer:set_anchor(0, 0)

    local logo = go.sprite.new("res/logo.png")
    logo:set_pos(visible_rect.width/2, visible_rect.height/2)
    logo:set_scale(2.0)
    game_layer:add_child(logo)

    root:add_child(game_layer)

    print("try return the gamelayer")
    return game_layer.__cobj
end
