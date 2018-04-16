local seal2d = require "seal2d"
local C = require "seal2d_primitive"

local parent = require "seal2d.game_object.node"

local primitive = {
    init = C.init,
    add_line = C.add_line,
    add_rect = C.add_rect
}

setmetatable(primitive, {__index = parent})

function primitive.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = primitive})
    return obj
end

return primitive
