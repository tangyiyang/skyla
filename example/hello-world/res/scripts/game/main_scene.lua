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

    local i = 0;

    local function loop(t)
        timer.new(t, function()
            print(string.format("time up %d, time = %s", i, os.date("%M:%S", os.time())))
            i = i + 1
            loop(t)
        end)
    end

    loop(1.0)
    loop(2.0)

end

return main_scene