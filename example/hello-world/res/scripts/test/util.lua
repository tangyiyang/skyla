local button = require "skyla.gui.button"

local util = {}

function util.create_test_button(text, callback)
    return button.new {
                        normal = "#ui_button_tiny.png",
                        text = text,
                        font = "fonts/animated_32_ffffff.fnt",
                        callback = callback
                    }
end

return util