local node_editor = {}

function node_editor.init()
    print("call node_editor.init")

    local editing_node
    skyla.dispatcher:on("on_node_clicked", function(_, node, phase, x, y)
        printf("node = %s, phase = %d, x, y = %.2f, %.2f", node:get_id(), phase, x, y)

        editing_node = node
    end)


    skyla.dispatcher:on("on_render_game_scene", function()

        if editing_node then
            --translate the node position to game_scene window and draw a rect
        end

    end)
end

return node_editor