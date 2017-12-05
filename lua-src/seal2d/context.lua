local seal2d = require "seal2d"
local C = require "seal2d_context"
local node = require "seal2d.game_object.node"

local context = {
    __cobj = seal2d.get_context(),
    __root = nil,
    get_visible_rect = C.get_visible_rect
}

function context:get_root()
    if not self.__root then
        self.__root = node.new {
            __cobj = C.get_root()
        }
    end
    return self.__root
end

return context