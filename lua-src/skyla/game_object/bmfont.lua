local skyla = require "skyla"
local C = require "skyla_bmfont"

local parent = require "skyla.game_object.node"

local bmfont = {
    init = C.init,
    set_text = C.set_text
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