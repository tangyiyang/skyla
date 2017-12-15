local node = require "seal2d.game_object.node"

local main_scene = class("main_scene", function()
    return node.new()
end)

function main_scene:ctor()
    local button = require "seal2d.gui.button"
    local b = button.new {
        normal = "images/ui_button_middle.png",
        text = "click me",
        font = "fonts/animated_32_ffffff.fnt",
        callback = function()
            print("responsed.")
        end
    }

    b:set_pos(100, 100)

    self:add_child(b)

    local timer = require "seal2d.base.timer"

    local t = timer.new(1.0, function()
            print("time up, time = ", os.date("%M:%S", os.time()))
        end, 100)
    t:start()

    local r = timer.new(3.0, function()
            print("time up, time = ", os.date("%M:%S", os.time()))
        end, 100)
    r:start()
end

return main_scene