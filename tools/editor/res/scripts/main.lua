package.path = package.path .. ";res/scripts/?.lua"

require "imgui_consts"
_G["editor"] = require "editor"
xpcall(editor.start, function(err, msg)
    print(err)
    print(debug.traceback(2, msg))
end)
