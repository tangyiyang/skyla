local director = {}

local scene_create_func
local update_func
local destroy_func
local game_objects = {}

local function on_start()
    local ctx = require "skyla.context"
    local root = ctx:get_root()

    scene_create_func(root)

    _G["go"].ctx = ctx
end

local function on_update(dt)
    for i = 1, #game_objects do
        game_objects[i]:__update(dt)
    end

    update_func(dt)
end

local function on_destroy()
    destroy_func()
end

function director.start(create_func, update_callback, destroy_callback)
    print("*** Entering Lua Scripts ***")

    _G["skyla"] = require "skyla"
    _G["misc"] = require "skyla.misc"
    _G["consts"] = require "skyla.base.consts"
    _G["go"] = require "skyla.game_object.go"

    skyla.texture = require("skyla.renderer.texture")
    skyla.dispatcher = require("skyla.event_dispatcher"){}


    local function inject(mod)
        for k, v in pairs(mod) do
            _G[k] = v
        end
    end

    inject(misc)
    inject(consts)
    inject(go)

    skyla.inject {
        on_start = on_start,
        on_update = on_update,
        on_destroy = on_destroy,
    }

    scene_create_func = create_func
    update_func = update_callback
    destroy_func = destroy_callback
end

function director.up_stage(go)
    game_objects[#game_objects + 1] = go
end

return director