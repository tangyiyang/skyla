local skyla = require "skyla"
local C = require "skyla_sprite"

local parent = require "skyla.game_object.node"

local sprite = {
    load_frames = C.load_frames,

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
