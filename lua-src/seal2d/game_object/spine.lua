local seal2d = require "seal2d"
local C = require "seal2d_spine"

local parent = require "seal2d.game_object.node"

local spine = {
    init = C.init,
    set_anim = C.set_anim,
}

setmetatable(spine, {__index = parent})

function spine.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = spine})
    return obj
end

return spine
