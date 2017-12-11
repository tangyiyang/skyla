local stm = require "seal2d.base.stm"

local scene_state = stm.create {
    { name = "start_test", from = "main_scene", to = "test_scene" },
    { name = "back_to_main", from = "test_scene", to = "main_scene" },
}

return scene_state