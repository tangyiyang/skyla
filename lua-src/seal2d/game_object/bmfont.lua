local seal2d = require "seal2d"
local C = require "seal2d_bmfont"

local parent = require "seal2d.game_object.node"

local bmfont = {
    init = C.init,
}

setmetatable(bmfont, {__index = parent})

function bmfont.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = bmfont})
    return obj
end

return bmfont