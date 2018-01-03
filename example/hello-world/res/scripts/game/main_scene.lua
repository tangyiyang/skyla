local node = require "seal2d.game_object.node"

local main_scene = class("main_scene", function()
    return node.new()
end)

function main_scene:ctor()
    -- local button = require "seal2d.gui.button"
    -- local b = button.new {
    --     normal = "images/ui_button_middle.png",
    --     text = "click me",
    --     font = "fonts/animated_32_ffffff.fnt",
    --     callback = function()
    --         print("responsed.")
    --     end
    -- }

    -- b:set_pos(100, 100)

    -- self:add_child(b)

    local sprite = require "seal2d.game_object.sprite"

    local view_width = 200
    local view_height = 400

    local big_sprite = sprite.new("images/ui_button_middle.png")
    local sprite_w, sprite_h = big_sprite:get_size()
    -- big_sprite:set_scale(100/sprite_w, view_height/sprite_h)
    -- self:add_child(big_sprite)
    big_sprite:set_anchor(0, 0)
    -- big_sprite:set_pos(100, 100)
    -- big_sprite:set_scale(3)
    local action = require "seal2d_action"
    -- big_sprite:run_action(action.move_to(20.0, 100, 500))
    -- self:add_child(big_sprite)
    local big_sprite_2 = sprite.new("images/ui_button_middle.png")
    big_sprite_2:set_pos(sprite_w, 0)
    big_sprite_2:set_anchor(0, 0)
    -- big_sprite_2:set_scale(2)
    -- big_sprite_2:set_scale(100/sprite_w, view_height/sprite_h)
    big_sprite:add_child(big_sprite_2)

    -- local x, y, w, h = big_sprite:get_bounding_box()
    -- print("x, y, w, h = ", x, y, w, h)

    local scroll_view = require "seal2d.gui.scroll_view"

    local v = scroll_view.new {
                                width = view_width,
                                height = view_height
                            }
    v:set_anchor(0.5, 0.5)
    v:set_pos(200, 200)
    v:add_content(big_sprite)
    self:add_child(v)
end

return main_scene