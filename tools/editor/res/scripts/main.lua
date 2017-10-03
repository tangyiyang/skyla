package.path = package.path .. ";res/scripts/?.lua"

local function initialize()
    -- the order of calling these functions matters!
    imgui = require "imgui"
    require "imgui_consts"
    require "config"
    editor = require "editor"
end

initialize()

xpcall(editor.start, function(err, msg)
    print(err)
    print(debug.traceback(2, msg))
end)
