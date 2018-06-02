local skyla = require "skyla"
local C = require "skyla_primitive"

local parent = require "skyla.game_object.node"

local primitive = {
    init = C.init,
    add_line = C.add_line,
    add_rect = C.add_rect
}

setmetatable(primitive, {__index = parent})

function primitive.new(...)
    local obj = {
        __cobj = C.new(...),
        __children = {},
    }
    setmetatable(obj, {__index = primitive})
    return obj
end

return primitive
