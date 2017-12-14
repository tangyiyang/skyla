local function main()
	package.path = package.path .. ";res/scripts/?.lua"
                                .. ";lua-src/?.lua"
	local director = require "seal2d.director"
	director.start(function(root)
		local util = require "seal2d_util"
		util.add_search_path("res/")

		print("start...")
		local main_scene = require "game.main_scene"
		print("start...1 ")
		root:add_child(main_scene.new())
		print("start...2 ")
	end)
end

main()