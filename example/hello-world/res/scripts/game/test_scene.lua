local node = require "seal2d.game_object.node"

local test_scene = class("test_scene", function()
    return node.new()
end)


local function bounding_box_test(self)
    local sprite = require "seal2d.game_object.sprite"
    local a = sprite.new("images/ui_button_middle.png")
    a:set_pos(100, 100)
    self:add_child(a)

    local x, y, w, h = a:get_bounding_box()

    local b = sprite.new("images/ui_button_middle.png")
    b:set_pos(200, 200)
    a:add_child(b)

    x, y, w, h = a:get_bounding_box()
end

local function list_view_test(self)
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

local function primitive_basic_test(self)
    local primitive = require("seal2d.game_object.primitive")

    local p = primitive.new()
    p:add_line(0, 0, 100, 100, 0xff0000ff);
    self:add_child(p)
end

function test_scene:ctor()
    -- bounding_box_test(self)
    -- list_view_test(self)
    primitive_basic_test(self)
end


return test_scene