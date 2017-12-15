local C = require "seal2d_timer"
local timer = {
    start = C.start,
    stop = C.stop,
}

function timer.new(interval, callback, loop)
    local t = {
        __cobj = C.new(interval, callback, loop)
    }

    setmetatable(t, {__index = timer})
    return t
end

return timer