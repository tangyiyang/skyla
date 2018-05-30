local skyla = require "skyla"
local director = require "skyla.director"
local C = require "skyla_node"

local node = {
    init = C.init,

    on_touch = C.on_touch,
    on_update = C.on_update,

    local_to_world = C.local_to_world,

    run_action = C.run_action,
    stop_all_actions = C.stop_all_actions,

    set_anchor = C.set_anchor,
    set_pos = C.set_pos,
    set_rotation = C.set_rotation,
    set_scale = C.set_scale,
    set_size = C.set_size,
    set_visible = C.set_visible,

    get_id = C.get_id,
    get_size = C.get_size,
    get_pos = C.get_pos,
    get_bounding_box = C.get_bounding_box,

    remove_all_children = C.remove_all_children
}

function node.new(t)
    local obj = t or {
        __cobj = C.new(),
        __parent = nil,
    }
    setmetatable(obj, {__index = node})
    return obj
end

function node:add_child(child, zorder)
    assert(child.__parent == nil)
    child.__parent = self
    C.add_child(self.__cobj, child.__cobj, zorder or 0)
end

function node:get_children()
    local children = {}
    local __children = C.get_children(self.__cobj)
    for i = 1, #__children do
        local c = {
            __cobj = __children[i],
            __parent = self,
        }
        setmetatable(c, {__index = node})
        children[#children+1] = c
    end
    return children
end

function node:up_stage()
    director.up_stage(self)
end

function node:set_debug_aabb_visible(visible, recursive)
    if not self.__debug_aabb_node then
        local primitive = require "skyla.game_object.primitive"
        local box = primitive.new()
        local w, h = self:get_size()
        box:add_rect(0, 0, w, h, COLOR_RED)
        self:add_child(box)
        self.__debug_aabb_node = box
    end

    self.__debug_aabb_node:set_visible(visible)

    if recursive then

    end
end

return node