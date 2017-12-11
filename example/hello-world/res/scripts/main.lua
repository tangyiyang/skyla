local seal2d = require "seal2d"
local util = require "seal2d_util"

local function on_start()
	local ctx = require "seal2d.context"

	util.add_search_path("res/")

	local root = ctx:get_root()
	local r = ctx:get_visible_rect()

	-- local sprite = require "seal2d.game_object.sprite"
 --    local s = sprite.new("images/ui_button_middle.png")
 --    s:set_pos(200, 200)
 --    root:add_child(s)

	local main_scene = require "game.main_scene"
	root:add_child(main_scene.new())

end

local function on_update(dt)
	print("call on_update", dt)
end

local function on_destroy()
	print("call on destroy")
end

local function inject(mod)
	for k, v in pairs(mod) do
		_G[k] = v
	end
end

local function main()
	print("*** Entering Lua Scripts ***")

	package.path = package.path .. ";res/scripts/?.lua"
								.. ";lua-src/?.lua"
	_G["seal2d"] = require "seal2d"
	_G["print_r"] = require "seal2d.print_r"
	_G["misc"] = require "seal2d.misc"

	inject(misc)

	seal2d.inject {
		on_start = on_start,
		on_update = on_update,
		on_destroy = on_destroy,
	}

	seal2d = require "seal2d"
end

main()