local seal2d = require "seal2d"
local C = require "seal2d_scale9sprite"

local parent = require "seal2d.game_object.node"

local scale9sprite = {
    init = C.init,
    set_texture = C.set_texture,
    set_border = C.set_border,
}

setmetatable(scale9sprite, {__index = parent})

function scale9sprite.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = scale9sprite})
    return obj
end

return scale9sprite
