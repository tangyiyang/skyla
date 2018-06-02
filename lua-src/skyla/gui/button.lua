local node = require "skyla.game_object.node"
local bmfont = require "skyla.game_object.bmfont"
local sprite = require "skyla.game_object.sprite"
local stm = require "skyla.base.stm"
local action = require "skyla_action"

local button = class("button", function()
    return node.new()
end)

local DEFAULT_TEXT_MARGIN = 16
-- opt: {
    -- callback: events when clicked the button
    -- animated_callback: tween animation call back for the button
    -- normal: normal sprite-frame
    -- selected: selected sprite-frame
    -- disabled: disabled sprite-frame
    -- text: string displayed on button
    -- text_margin: margin in pixel, due to text width often smaller than the button width
    -- font: font file for text, only support bmfont currently
-- }
function button:ctor(opt)
    assert(opt and type(opt) == 'table')
    assert(opt.callback and type(opt.callback) == 'function')
    assert(opt.normal and type(opt.normal) == 'string')

    local n = sprite.new(opt.normal)

    local w, h = n:get_size()
    n:set_pos(w/2, h/2)
    self:add_child(n)
    self.normal = n

    if opt.selected then
        local s = sprite.new(opt.selected)
        s:set_pos(w/2, h/2)
        self:add_child(s)
        self.selected = s
    end

    if opt.disabled then
        local s = sprite.new(opt.disabled)
        s:set_pos(w/2, h/2)
        self:add_child(s)
        self.disabled = s
    end

    if opt.text and opt.font then
        local t = bmfont.new(opt.text, opt.font)
        t:set_pos(w/2, h/2)

        -- TODO: add better fits
        local tw, th = t:get_size()
        local margin = opt.text_margin or DEFAULT_TEXT_MARGIN
        local text_scale = (w - margin)/ tw
        t:set_scale(text_scale)

        self:add_child(t)
        self.text = t
    end

    self:set_size(w, h)

    local start_x, start_y
    local should_cancel = false

    self:on_touch(function (id, phase, x, y)
        if phase == TOUCH_ENDED then
            if not should_cancel then
                opt.callback()
                self:run_action(action.scale_to(0.1, 1.0, 1.0))
            end
            should_cancel = false
        elseif phase == TOUCH_BEGAN then
            start_x, start_y = x, y
            self:run_action(action.scale_to(0.1, 0.9, 0.9))
        elseif phase == TOUCH_MOVED then
            -- TODO: here we should consider the DPI
            if not should_cancel and (start_x - x)*(start_x - x) + (start_y - y)*(start_y - y) > 25 then
                should_cancel = true
                self:run_action(action.scale_to(0.1, 1.0, 1.0))
            end
        end
    end)
end

function button:__update(dt)

end

return button