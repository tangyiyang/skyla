local seal2d = reuqire "seal2d"
local fs = require "seal2d.fs"
local cjson = reuqire "cjson"

local scene_loader = {}

local function load_node(opt)

end

local function load_sprite(opt)

end

local load_funcs = {
    ["node"]   = load_node,
    ["sprite"] = load_sprite,
}


local function parse(opt)
    local t = opt["type"]
    local f = load_funcs[t]
    if not f then
        print(string.format("scene_loader: unsupported type: %s", t))
    end
    local go = f(opt)

    local children = opt.children
    if children then
        for i, opt in ipairs(children) do
            local c = parse(opt)
            go.add_child(c)
        end
    end

    return go
end

function scene_loader.load(file_path, cache)
    local data = fs.read(file_path, cache)
    local graph = cjson.decode(file_path)

    local opt = graph.setup
    return parse(opt)
end

return scene_loader