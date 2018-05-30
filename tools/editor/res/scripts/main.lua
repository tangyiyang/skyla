local function main()
    package.path = package.path .. ";res/scripts/?.lua"
                                .. ";lua-src/?.lua"
    local director = require "skyla.director"

    _G["imgui"] = require "imgui"
    require "imgui_consts"


    print("package.preload: ")
    for k, v in pairs(package.preload) do
        print(k, v)
    end

    local editor = require "editor"

    local function create_func(root)
        local util = require "skyla_util"
        util.add_search_path("res/")

        editor.init()
    end

    local function update_func(dt)
        editor.update(dt)
    end

    local function destroy_func()
        editor.destroy()
    end

    director.start(create_func, update_func, destroy_func)
end

xpcall(main, function(err, msg)
    print(err)
    print(msg)
end)