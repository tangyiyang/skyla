local seal2d = require "seal2d"

local function on_init()
	print("call on_init")
	local s = seal2d.sprite("res/seal2d-transparent.png")
	s:set_pos(100, 100)

end

local function on_update(dt)
	print("call on_update", dt)
end

local function on_destroy()
	print("call on destroy")
end

local function trace_back(err, msg)
	print("traceback: ", err)
	print(msg)
end

local function main()
	print("hello world")
	seal2d.inject {
		on_init = on_init,
		on_update = on_update,
		on_destroy = on_destroy,
	}
end

main()

