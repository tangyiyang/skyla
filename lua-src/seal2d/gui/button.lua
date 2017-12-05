local node = require "seal2d.game_object.node"
local stm = require "seal2d.base.stm"

local button = class("button", function()
    return node.new()
end)

-- opt: {
    -- callback: events when clicked the button
    -- animated_callback: tween animation call back for the button
    -- normal: normal sprite-frame
    -- selected: selected sprite-frame
    -- disable: disabled sprite-frame
    -- text: string displayed on button
    -- font: font file for text, only support bmfont currently
    -- font_size: size for text
-- }
function button:ctor(opt)
    assert(opt and type(opt) == 'table')
    assert(opt.callback and type(opt.callback) == 'function')
    assert(opt.normal and type(opt.normal) == 'string')

end


return button