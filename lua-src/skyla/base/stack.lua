local stack = {}

function stack.new()
    local s = {
        __elements = {}
    }
    setmetatable(s, {__index = stack} )
    return s
end

function stack:push(element)
    self.__elements[#self.__elements + 1] = element
end

function stack:pop()
    self.__elements[#self.__elements] = nil
end

function stack:top()
    return self.__elements[#self.__elements]
end

function stack:concat(sep, from, to)
    return table.concat(self.__elements, sep, from, to)
end

function stack:length()
    return #self.__elements
end

function stack:visit(func)
    for i = #self.__elements, 1, -1 do
        func(self.__elements[i])
    end
end

function stack:empty()
    return self:length() == 0
end

return stack