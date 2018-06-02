local skyla = require "skyla"
local C = require "skyla_context"
local node = require "skyla.game_object.node"

local context = {
    __cobj = skyla.get_context(),
    __root = nil,
    get_visible_rect = C.get_visible_rect
}

function context:get_root()
    if not self.__root then
        self.__root = node.new {
            __cobj = C.get_root(),
            __children = {}
        }
    end
    return self.__root
end

return context