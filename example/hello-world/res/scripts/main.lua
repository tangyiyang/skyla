local seal2d = require "seal2d"

local function on_start()
	local ctx = seal2d.context()
	local root = ctx:get_root()

	local r = ctx:get_visible_rect()
	local s = seal2d.sprite("res/seal2d-transparent.png")
	s:set_pos(r.width/2, r.height/2);
	s:set_color(0xffff00ff);

	root:add_child(s)
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
		on_start = on_start,
		on_update = on_update,
		on_destroy = on_destroy,
	}
end

main()
