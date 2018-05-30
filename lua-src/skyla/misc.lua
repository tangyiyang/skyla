local misc = {}

function misc.class(name, super)
    local __class = {}
    if type(super) == 'table' or type(super) == nil then
        __class.new = function(...)
            local this = {}
            setmetatable(this, {__index = __class})
            this.__name = name
            this:ctor(...)
            return this
        end
    elseif type(super == 'function') then
        __class.new = function(...)

            local obj = super(...)

            for k,v in pairs(__class) do
                obj[k] = v
            end
            obj.__name = name
            obj.__ctor = __class.ctor
            obj:__ctor(...)
            return obj
        end
    else
        assert(false, "invalid super type")
    end

    return __class
end


local log_level_tag = {
    [1] = 'D',
    [2] = 'I',
    [3] = 'W',
    [4] = 'E',
 }

function misc.log(level, ...)
    print(log_level_tag[level] .. '\t', ...)
end


function misc.print_r(root)
    if not root then
        print("attempt to print an empty table.")
        print(debug.traceback("", 2))
        return
    end
    if type(root) ~= "table" then
        print(string.format("attempt to print a table, but it's type = %s, value = %s.",
                type(root), tostring(root)))
        return
    end

    local cache = { [root] = "." }
    local function _dump(t,space,name)
          local temp = {}
          for k,v in pairs(t) do
              local key = tostring(k)
              if cache[v] then
                  table.insert(temp,"+" .. key .. " {" .. cache[v].."}")
              elseif type(v) == "table" then
                  local new_key = name .. "." .. key
                  cache[v] = new_key
                  table.insert(temp,"+" .. key .. _dump(v,space .. (next(t,k) and "|" or " " )
                                    .. string.rep(" ",#key),new_key))
              else
                  table.insert(temp,"+" .. key .. " [" .. tostring(v).."]")
              end
          end
          return table.concat(temp,"\n"..space)
    end

    local info =  "\n-----------------------------------------------------\n" ..
                    _dump(root, "","") ..
                  "\n-----------------------------------------------------\n"
    io.write(info)
    io.flush()
end

function misc.printf(...)
    print(string.format(...))
end

function misc.contains(rect, x, y)
    return  x >= rect.x and
            x <= rect.x + rect.width and
            y >= rect.y and
            y <= rect.y + rect.height
end

function misc.nkeys(t)
    local total = 0
    for _,_ in pairs(t) do
        total = total + 1
    end
    return total
end

function misc.printf(...)
    print(string.format(...))
end

function misc.tobytes(s)
    local t = {}
    local tinsert = table.insert
    for i = 1, #s do
        tinsert(t, string.byte(s, i))
    end
    return table.concat(t, " ")
end

function misc.shuffle_t(t)
    return misc.shuffle(#t)
end

function misc.shuffle(total)
    assert(total)
    local base = {}
    local results = {}
    for i = 1, total do
        base[i] = i
    end
    for i = 1, total do
        local k = math.random(i, total)
        base[i], base[k] =  base[k],  base[i]
    end
    for i = 1, total do
        results[i] = base[i]
    end
    return results
end

function misc.order_pairs(t)
    local tinsert = table.insert

    local ot = {}
    for k, _ in pairs(t) do
        tinsert(ot, k)
    end

    table.sort(ot, function(a, b)
        return tostring(a) < tostring(b)
    end)

    local index = 0
    local function iter()
        index = index + 1
        if t[ot[index]] then
            return index, t[ot[index]]
        end
    end

    return iter, t, nil
end

function misc.shuffle_pairs(t)
    local tinsert = table.insert
    local ot = {}
    for k, _ in pairs(t) do
        tinsert(ot, k)
    end

    local shuffle_t = misc.shuffle_t(ot)

    local index = 0
    local function iter()
        index = index + 1
        if t[shuffle_t[index]] then
            return index, t[shuffle_t[index]]
        end
    end

    return iter, t, nil
end

return misc