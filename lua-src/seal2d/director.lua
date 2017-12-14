local director = {}

local scene_create_func
local game_objects = {}

local function on_start()
    local ctx = require "seal2d.context"
    local root = ctx:get_root()

    scene_create_func(root)
end

local function on_update(dt)
    for i = 1, #game_objects do
        game_objects[i]:__update(dt)
    end
end

local function on_destroy()

end

function director.start(create_func)
    print("*** Entering Lua Scripts ***")

    _G["seal2d"] = require "seal2d"
    _G["print_r"] = require "seal2d.print_r"
    _G["misc"] = require "seal2d.misc"
    _G["consts"] = require "seal2d.base.consts"

    local function inject(mod)
        for k, v in pairs(mod) do
            _G[k] = v
        end
    end

    inject(misc)
    inject(consts)

    seal2d.inject {
        on_start = on_start,
        on_update = on_update,
        on_destroy = on_destroy,
    }

    scene_create_func = create_func
end

function director.up_stage(go)
    game_objects[#game_objects + 1] = go
end

return director