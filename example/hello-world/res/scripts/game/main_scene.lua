local node = require "seal2d.game_object.node"

local main_scene = class("main_scene", function()
    return node.new()
end)

local button = require "seal2d.gui.button"

function main_scene:ctor()
    -- local logo = seal2d.sprite("res/images/bunny.png")
    -- self:add_child(logo)


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
end


return main_scene