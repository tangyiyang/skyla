local skyla = require "skyla"
local C = require "skyla_texture"

local texture = {
    get_opengl_id = C.get_opengl_id,
    get_file_path = C.get_file_path
}

function texture.new_with_raw_obj(cobj)
    local obj = { __cobj = cobj }
    setmetatable(obj, {__index = texture})
    return obj
end

function texture.new(file_path)
    local cobj = C.new(file_path)
    return texture.new_with_raw_obj(cobj)
end

return texture