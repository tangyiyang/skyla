local vec_math = {}

function vec_math.distance(x0, y0, x1, y1)
    return sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1))
end

return vec_math