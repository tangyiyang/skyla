local node = require "seal2d.game_object.node"
local bmfont = require "seal2d.game_object.bmfont"
local sprite = require "seal2d.game_object.sprite"

local stm = require "seal2d.base.stm"

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


    self:on_touch(function()
        print("Lua ontouch")
    end)
end

return button