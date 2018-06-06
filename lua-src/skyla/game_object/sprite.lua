local skyla = require "skyla"
local C = require "skyla_sprite"

local parent = require "skyla.game_object.node"

local sprite = {
    load_frames = C.load_frames,

    init = C.init,
    set_texture = C.set_texture,
    set_color = C.set_color,
    set_blend_mode = C.set_blend_mode,
}

setmetatable(sprite, {__index = parent})

function sprite.new(...)
    local obj = {
        __cobj = C.new(...),
        __children = {},
    }
    setmetatable(obj, {__index = sprite})
    return obj
end

function sprite:get_texture()
    -- TODO:
    -- if the texture is loaded from other module, such as texture_cache
    -- the texture object may have many copies in Lua level.
    -- does this going to be a problem?
    if not self.__texture then
        print("self = ", self)
        print("skyla.texture = ", skyla.texture)
        self.__texture = skyla.texture.new_with_raw_obj(C.get_texture(self))
    end

    return self.__texture
end

return sprite
