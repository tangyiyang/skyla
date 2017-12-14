local seal2d = require "seal2d"
local C = require "seal2d_node"
local director = require "seal2d.director"

local node = {
    init = C.init,
    set_pos = C.set_pos,
    get_size = C.get_size,
    set_size = C.set_size,
    on_event = C.on_event,
}

function node.new(t)
    local obj = t or {
        __cobj = C.new(),
        __parent = nil,
    }
    setmetatable(obj, {__index = node})
    return obj
end

function node:add_child(child)
    assert(child.__parent == nil)
    child.__parent = self
    C.add_child(self.__cobj, child.__cobj)
end

function node:up_stage()
    director.up_stage(self)
end

return node