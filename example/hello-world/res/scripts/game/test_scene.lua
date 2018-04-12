local node = require "seal2d.game_object.node"

local test_scene = class("test_scene", function()
    return node.new()
end)

function test_scene:ctor()
    local list_view = require("seal2d.gui.list_view")
    local button = require "seal2d.gui.button"

    local function cell_create_func(index)
        print("call create func: index = ", index)
        local b = button.new {
                                normal = "images/ui_button_middle.png",
                                text = "click me",
                                font = "fonts/animated_32_ffffff.fnt",
                                callback = function()
                                    print("responsed.")
                                end
                            }
        return b
    end

    local list = list_view.new {
                                mode = "row",
                                width = 400, height = 150,
                                n_cell = 5,
                                cell_create_func = cell_create_func
                            }

    list:set_pos(300, 300)
    self:add_child(list)
end


return test_scene