local node = require "seal2d.game_object.node"
local bmfont = require "seal2d.game_object.bmfont"
local sprite = require "seal2d.game_object.sprite"
local stm = require "seal2d.base.stm"
local action = require "seal2d_action"

local button = class("button", function()
    return node.new()
end)

-- opt: {
    -- callback: events when clicked the button
    -- animated_callback: tween animation call back for the button
    -- normal: normal sprite-frame
    -- selected: selected sprite-frame
    -- disabled: disabled sprite-frame
    -- text: string displayed on button
    -- font: font file for text, only support bmfont currently
    -- font_size: size for text
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
        self:add_child(t)
        self.text = t
    end

    self:set_size(w, h)

    local start_x, start_y
    local should_cancel = false
    local function on_touch(id, phase, x, y)
        print("id, phase, x, y = ", id, phase, x, y)

        if phase == TOUCH_ENDED then
            if not should_cancel then
                opt.callback()
                self:run_action(action.scale_to(0.1, 1.0, 1.0))
            end
            should_cancel = false
        elseif phase == TOUCH_BEGIN then
            start_x, start_y = x, y
            self:run_action(action.scale_to(0.1, 0.9, 0.9))
        elseif phase == TOUCH_MOVED then
            -- TODO: here we should consider the DPI
            if not should_cancel and (start_x - x)*(start_x - x) + (start_y - y)*(start_y - y) > 25 then
                should_cancel = true
                self:run_action(action.scale_to(0.1, 1.0, 1.0))
            end
        end
    end

    self:on_event(function(event_type, ...)
        if event_type == "seal2d_event_touch" then
            on_touch(...)
        end
    end)
end

function button:__update(dt)

end

return button