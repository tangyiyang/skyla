local skyla = require "skyla"
local C = require "skyla_texture"

local texture = {
    get_texture_opengl_id = C.get_texture_opengl_id
}

function texture.new(...)
    local obj = {
        __cobj = C.new(...)
    }
    setmetatable(obj, {__index = texture})
    return obj
end

return texture