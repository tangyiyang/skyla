local node = require "seal2d.game_object.node"
local sprite = require "seal2d.game_object.sprite"

local util = require "test.util"

local sprite_test_layer = class("sprite_test_layer", function()
    return node.new()
end)

function sprite_test_layer:ctor()
    local visible_rect = require("seal2d.context"):get_visible_rect()

    local btn = util.create_test_button("add", function()
        local s = sprite.new("#prist_magicball.png")
        s:set_pos(math.random(visible_rect.width), math.random(visible_rect.height))
        self:add_child(s)
    end)

    btn:set_anchor(0.5, 1)
    btn:set_pos(visible_rect.width/2, visible_rect.height)
    self:add_child(btn)
end

return sprite_test_layer