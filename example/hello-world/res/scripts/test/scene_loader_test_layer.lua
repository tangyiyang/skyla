local util = require "test.util"

local scene_loader_test_layer = class("scene_loader_test_layer", function()
    return go.node.new()
end)

function scene_loader_test_layer:ctor()
    local scene_loader = require "skyla.scene_loader"

    require("skyla_util").add_search_path("example_project/")
    local s = scene_loader.load("example_project/scene/scene_main.json")
    self:add_child(s)

end

return scene_loader_test_layer