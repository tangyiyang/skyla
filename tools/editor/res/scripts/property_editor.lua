local imgui_helper = require "imgui_helper"

local property_editor = {}

local editing_node

function property_editor.init()
    skyla.dispatcher:on("on_editing_node_changed", function(_, new_node)
        editing_node = new_node
    end)
end

function property_editor.render()
    if editing_node then
        imgui.Begin("Property")

        -- Everybody needs "Code" and "Node"
        if imgui.CollapsingHeader("Code") then
            local ok, text
            ok, text = imgui.InputText("script", "")
            if ok then
                print("script = ", text)
                editing_node._opt.script = text
            end

            imgui.InputText("var", "")
        end

        if imgui.CollapsingHeader("Node") then
            local checked = imgui.Checkbox("Visible", editing_node:is_visible())
            editing_node:set_visible(checked)
            editing_node._opt.visible = checked

            local x, y = editing_node:get_pos()

            imgui_helper.decimal_input("x", x, function(value)
                editing_node._opt.position.x = value
                editing_node:set_pos(value, y)
            end)

            imgui_helper.decimal_input("y", y, function(value)
                editing_node._opt.position.y = value
                editing_node:set_pos(x, value)
            end)

            local scale_x, scale_y = editing_node:get_scale()

            imgui_helper.decimal_input("scale_x", scale_x, function(value)
                editing_node._opt.scale.x = value
                editing_node:set_scale(value, scale_y)
            end)

            imgui_helper.decimal_input("scale_y", scale_y, function(value)
                editing_node._opt.scale.y = value
                editing_node:set_scale(scale_x, value)
            end)

            local rotation = editing_node:get_rotation()
            imgui_helper.decimal_input("rotation", rotation, function(value)
                editing_node._opt.rotation = value
                editing_node:set_rotation(value)
            end)

            local anchor_x, anchor_y = editing_node:get_anchor()

            imgui_helper.decimal_input("anchor_x", anchor_x, function(value)
                editing_node._opt.anchor.x = value
                editing_node:set_anchor(value, anchor_y)
            end)

            imgui_helper.decimal_input("anchor_y", anchor_y, function(value)
                editing_node._opt.anchor.y = value
                editing_node:set_anchor(anchor_x, value)
            end)
        end

        if editing_node.type == "Sprite" then

        end

        imgui.End()
    end
end

return property_editor