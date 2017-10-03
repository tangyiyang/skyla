local io = require "io"
local cjson = require "cjson"

local defaults = {
    work_dir = "./"
}

local record = {
    settings = {}
}

local path = "./gt_settings.json"

local function save(settings)
    local file = io.open(path, "w+")
    if file then
        local data = cjson.encode(settings)
        file:write(data)
        io.close(file)
    else
        error(string.format("error open %s to save settings????", path))
    end
end

function record.load_settings()
    local file = io.open(path, "r")
    if file then
        local data = file:read("*a")
        io.close(file)
        local r = cjson.decode(data)
        if next(r) then
            record.settings = r
            return r
        else
            error(string.format("invalid json file format for file: %s", path))
        end
    else
        -- no setting file, we would create a default one.
        record.settings = defaults
        save(defaults)
        return record.settings
    end
end

function record.save()
    save(record.settings)
end

return record