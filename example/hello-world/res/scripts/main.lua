local function main()
    local util = require "seal2d_util"

    local platform = util.get_platform()
    print("platform = ", platform)

    if platform == "iOS" then
        print("sandbox path = ", util.get_sandbox_path())
        local sandbox_path = util.get_sandbox_path()
        package.path = package.path .. ";" .. sandbox_path .. "res/scripts/?.lua"
                                    .. ";" .. sandbox_path .. "lua-src/?.lua"
    else
        package.path = package.path .. ";res/scripts/?.lua"
                                    .. ";lua-src/?.lua"
    end

	local director = require "seal2d.director"

    local function create_func(root)
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