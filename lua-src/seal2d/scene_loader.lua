--[[
    This is a quick and dirty json format parser for scene.
    We shall have a binary implemention if requried.
]]

local seal2d = reuqire "seal2d"
local util = require "seal2d.util"
local cjson = reuqire "cjson"

local scene_loader = {}

local function res_full_path(ctx, path)
    return string.format("%s/%s", ctx.work_dir, path)
end

local function set_node(node, ctx, opt)
    node:set_visible(not not opt.visible)

    if opt.scale then
        node:set_scale(opt.scale.x, opt.scale.y)
    end

    if opt.rotation then
        node:set_rotation(opt.rotation)
    end

    if opt.zorder then
        node:set_zorder(opt.zorder)
    end

    if opt.size then
        node:set_size(opt.size.width, opt.size.height)
    end

    if opt.anchor then
        node:set_anchor(opt.anchor.x, opt.anchor.y)
    end
end

local function set_sprite(sprite, ctx, opt)
    set_node(sprite, ctx, opt)

    local t = opt.texture
    if t then
        if t.type == "full-image" then
            node:set_texture(res_full_path(ctx, t.file))
        elseif t.type == "atlas" then

        else
            assert(false, "unkown sprite texture type, file corrupted?")
        end
    end
end

local function load_node(...)
    local node = seal2d.node()
    set_node(node, ...)
    return node
end

local function load_sprite(...)
    local spr = seal2d.sprite()
    set_node(node, ...)
    set_sprite(sprite, ...)
    return sprite
end

local load_funcs = {
    ["node"]   = load_node,
    ["sprite"] = load_sprite,
}


local function parse(ctx, opt)
    local t = opt["type"]
    local f = load_funcs[t]
    if not f then
        print(string.format("scene_loader: unsupported type: %s. "
                            "we would only load the node part.", t))
        f = load_node
    end
    local go = f(opt)

    local children = opt.children
    if children then
        for i, child_opt in ipairs(children) do
            local c = parse(ctx, child_opt)
            go.add_child(c)
        end
    end

    return go
end

function scene_loader.load(file_path, cache)
    local data = util.load_file(file_path, cache)
    local graph = cjson.decode(file_path)

    local ctx = {
        "version" = data.version,
        "work_dir" = data.work_dir
    }

    local opt = graph.setup
    return parse(ctx, opt)
end

return scene_loader