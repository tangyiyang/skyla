local main_scene = class("main_scene", function()
    return seal2d.node()
end)

function main_scene:ctor()

    self.fucker = "123"
    local logo = seal2d.sprite("res/images/bunny.png")
    self:add_child(logo)
end


return main_scene