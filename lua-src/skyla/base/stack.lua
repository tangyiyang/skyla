local stack = {}

function stack.new()
    local s = {
        __n = 0,
        __elements = {}
    }
    setmetatable(s, {__index = stack} )
    return s
end

function stack:push(element)
    self.__elements[#self.__elements + 1] = element
    self.__n = self.__n + 1
end

function stack:pop()
    self.__elements[self.__n] = nil
    self.__n = self.__n - 1
end

function stack:top()
    return self.__elements[self.__n]
end

function stack:concat(sep)
    return table.concat(self.__elements, sep)
end

return stack