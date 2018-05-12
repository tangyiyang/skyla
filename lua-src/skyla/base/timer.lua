local C = require "skyla_timer"
local timer = {
    start = C.start,
    stop = C.stop,
}

function timer.new(interval, callback)
    local t = {
        __cobj = C.new(interval, callback)
    }

    setmetatable(t, {__index = timer})
    t:start()
    return t
end

return timer