local util = require "test.util"

local sprite_test_layer = class("sprite_test_layer", function()
    return go.node.new()
end)

local function spawn(self)
    local visible_rect = require("seal2d.context"):get_visible_rect()
    local s = go.sprite.new("#prist_magicball.png")
    s:set_pos(math.random(visible_rect.width), math.random(visible_rect.height))
    self:add_child(s)
end

function sprite_test_layer:ctor()
    local visible_rect = require("seal2d.context"):get_visible_rect()
    self:set_anchor(0, 0)
    self:set_size(visible_rect.width, visible_rect.height)

    local label = go.bmfont.new(
        "touch/click to spawn",
        "fonts/animated_32_ffffff.fnt"
    )

    label:set_pos(visible_rect.width/2, visible_rect.height)
    label:set_anchor(0.5, 1)

    self:add_child(label)

    local function on_touch(_, phase, x, y)
        if phase == TOUCH_ENDED then
            spawn(self)
        end
    end

    self:on_touch(function(event_type, ...)
        if event_type == "seal2d_event_touch" then
            on_touch(...)
        end
    end)
end

return sprite_test_layer