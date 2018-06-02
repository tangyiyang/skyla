local skyla = require "skyla"
local C = require "skyla_spine"

local parent = require "skyla.game_object.node"

local spine = {
    init = C.init,
    set_anim = C.set_anim,
    get_all_anim_names = C.get_all_anim_names,
}

setmetatable(spine, {__index = parent})

function spine.new(...)
    local obj = {
        __cobj = C.new(...),
        __children = {},
    }
    setmetatable(obj, {__index = spine})
    return obj
end

return spine
