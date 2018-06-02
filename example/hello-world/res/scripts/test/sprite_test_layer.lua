local util = require "test.util"

local sprite_test_layer = class("sprite_test_layer", function()
    return go.node.new()
end)

local mode = "click" -- click

local function negative_or_positive()
    return math.random(2) % 2 == 0 and 1 or -1
end

local function spawn(self, spawn_count, container)
    for i = 1, spawn_count do
        local visible_rect = require("skyla.context"):get_visible_rect()
        local s = go.sprite.new("res/images/bunny.png")
        s:set_pos(math.random(visible_rect.width), math.random(visible_rect.height))
        s.speed_x = math.random(200) * negative_or_positive()
        s.speed_y = math.random(200) * negative_or_positive()
        self:add_child(s)
        container[#container+1] = s
    end
end

function sprite_test_layer:ctor()
    local visible_rect = require("skyla.context"):get_visible_rect()
    self:set_anchor(0, 0)
    self:set_size(visible_rect.width, visible_rect.height)

    local label = go.bmfont.new(
        "touch/click to spawn",
        "fonts/animated_32_ffffff.fnt"
    )

    label:set_pos(visible_rect.width/2, visible_rect.height)
    label:set_anchor(0.5, 1)

    self:add_child(label, 1)

    local counter_label = go.bmfont.new(
        "count: ",
        "fonts/animated_32_ffffff.fnt"
    )

    counter_label:set_pos(visible_rect.width - 20, visible_rect.height)
    counter_label:set_anchor(1, 1)

    self:add_child(counter_label, 1)

    local begin_time_stamp
    local sprites = {}
    local w, h = visible_rect.width, visible_rect.height

    self:on_touch(function (_, phase, x, y)
        if phase == TOUCH_BEGAN then
            begin_time_stamp = os.time()
        elseif phase == TOUCH_ENDED then
            begin_time_stamp = nil
        end
    end)

    if mode == "auto" then
        local s = spawn(self, 2000, sprites)
        counter_label:set_text(string.format("count: %d", #sprites))
    end

    self:on_update(function(dt)
        if begin_time_stamp then
            if mode == "click" then
                local now = os.time()
                local spawn_count = 2 ^ (now - begin_time_stamp)
                local s = spawn(self, spawn_count, sprites)
                counter_label:set_text(string.format("count: %d", #sprites))
            end
        end

        for i = 1, #sprites do
            local s = sprites[i]
            local x, y = s:get_pos()
            local x_offset = s.speed_x * dt
            local y_offset = s.speed_y * dt
            local next_x, next_y = x + x_offset, y + y_offset
            if next_x >= w or next_x <= 0 then
                s.speed_x = -s.speed_x
            end

            if next_y >= h or next_y <= 0 then
                s.speed_y = -s.speed_y
            end

            local tangent = math.atan(s.speed_y / s.speed_x)
            s:set_rotation(tangent * 180  / 3.14 - 90)

            s:set_pos(next_x, next_y)
        end
    end)
end

return sprite_test_layer