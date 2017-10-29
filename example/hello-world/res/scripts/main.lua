local seal2d = require "seal2d"
local util = require "seal2d.util"

local function on_start()
	local scene_loader = require "seal2d.scene_loader"

	local ctx = seal2d.context()
	local root = ctx:get_root()
	util.add_search_path("res/")


	local scene = scene_loader.load("res/scene/scene_main.json")

	root:add_child(scene)
	-- local r = ctx:get_visible_rect()
	-- local s = seal2d.sprite("res/seal2d-transparent.png")
	-- s:set_pos(r.width/2, r.height/2);
	-- s:set_color(0xffff00ff);

	-- root:add_child(s)
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
	print("*** Entering Lua Scripts ***")

	package.path = package.path .. ";res/scripts/?.lua"
	seal2d.inject {
		on_start = on_start,
		on_update = on_update,
		on_destroy = on_destroy,
	}

	print_r = require "seal2d.print_r"
end

main()

