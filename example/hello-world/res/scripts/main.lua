local seal2d = require "seal2d"

local function on_start()
	print("call on_init")


	-- print("S = ", s)
	for k,v in pairs(seal2d) do
		print(k, v)
	end

	local ctx = seal2d.context()
	print("ctx = ", ctx)
	local root = ctx:get_root()

	local s = seal2d.sprite("res/seal2d-transparent.png")
	s:set_pos(100, 100);
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

