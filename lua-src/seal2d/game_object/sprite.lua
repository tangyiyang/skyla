local seal2d = require "seal2d"
local C = require "seal2d_sprite"

local parent = require "seal2d.game_object.node"

local sprite = {
    init = C.init,
    set_texture = C.set_texture,
}

setmetatable(sprite, {__index = parent})

function sprite.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = sprite})
    return obj
end

return sprite
