local function main()
	package.path = package.path .. ";res/scripts/?.lua"
                                .. ";lua-src/?.lua"
	local director = require "seal2d.director"

    local function create_func(root)
        local util = require "seal2d_util"
        util.add_search_path("res/")

        local test_scene = require "game.test_scene"
        root:add_child(test_scene.new())
    end

    local function on_update()

    end

    local function on_destroy()

    end

	director.start(create_func, on_update, on_destroy)
end

main()