local seal2d = require "seal2d"
local director = require "seal2d.director"
local C = require "seal2d_node"

local node = {
    init = C.init,
    on_event = C.on_event,
    run_action = C.run_action,
    stop_all_actions = C.stop_all_actions,

    set_anchor = C.set_anchor,
    set_pos = C.set_pos,
    set_scale = C.set_scale,
    set_size = C.set_size,

    get_size = C.get_size,
    get_pos = C.get_pos,

    get_bounding_box = C.get_bounding_box,
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