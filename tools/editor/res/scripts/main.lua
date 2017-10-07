package.path = package.path .. ";res/scripts/?.lua"

local function initialize()
    -- the order of calling these functions matters!
    imgui = require "imgui"
    require "imgui_consts"
    require "config"
    editor = require "editor"
    record = require "record"

    -- inject serveral global util functions.
    print_r = require "print_r"
end

initialize()

xpcall(editor.start, function(err, msg)
    print(err)
    print(debug.traceback(2, msg))
end)
