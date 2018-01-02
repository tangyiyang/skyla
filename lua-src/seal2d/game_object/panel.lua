local seal2d = require "seal2d"
local C = require "seal2d_panel"

local parent = require "seal2d.game_object.node"

local panel = {
    init = C.init,
}

setmetatable(panel, {__index = parent})

function panel.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = panel})
    return obj
end

return panel