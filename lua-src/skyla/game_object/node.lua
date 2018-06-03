local skyla = require "skyla"
local director = require "skyla.director"
local C = require "skyla_node"

local node = {
    init = C.init,

    on_touch = C.on_touch,
    on_update = C.on_update,

    local_to_world = C.local_to_world,

    get_bounding_box = C.get_bounding_box,
    get_id = C.get_id,
    get_size = C.get_size,
    get_pos = C.get_pos,
    is_visible = C.is_visible,

    remove_from_parent = C.remove_from_parent,
    remove_all_children = C.remove_all_children,

    run_action = C.run_action,
    stop_all_actions = C.stop_all_actions,

    set_anchor = C.set_anchor,
    set_pos = C.set_pos,
    set_rotation = C.set_rotation,
    set_scale = C.set_scale,
    set_size = C.set_size,
    set_visible = C.set_visible,
}

function node.new(t)
    local obj = t or {
        __cobj = C.new(),
        __parent = nil,
        __children = {},
    }
    setmetatable(obj, {__index = node})
    return obj
end

function node:add_child(child, zorder)
    assert(child.__parent == nil)
    child.__parent = self
    C.add_child(self.__cobj, child.__cobj, zorder or 0)
    self.__children[#self.__children+1] = child
end

function node:get_children()
    return self.__children
end

function node:up_stage()
    director.up_stage(self)
end

function node:set_debug_aabb_visible(visible)
    if not self.__debug_aabb_node then
        local primitive = require "skyla.game_object.primitive"
        local box = primitive.new()
        local w, h = self:get_size()
        box:add_rect(0, 0, w, h, COLOR_RED)
        self:add_child(box)
        self.__debug_aabb_node = box
    end

    self.__debug_aabb_node:set_visible(visible)
end

function node:set_opt(opt)
    print("set node opt, id = ", self:get_id(), self)
    self._opt = opt
end

function node:get_opt()
    print("get node opt, id = ", self:get_id(), self)
    return self._opt
end

return node