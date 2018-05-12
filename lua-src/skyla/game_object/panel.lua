local skyla = require "skyla"
local C = require "skyla_panel"

local parent = require "skyla.game_object.node"

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