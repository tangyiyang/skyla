local function main()
	package.path = package.path .. ";res/scripts/?.lua"
                                .. ";lua-src/?.lua"
	local director = require "seal2d.director"
	director.start(function(root)
		local util = require "seal2d_util"
		util.add_search_path("res/")

		local main_scene = require "game.main_scene"
		root:add_child(main_scene.new())
	end)
end

main()