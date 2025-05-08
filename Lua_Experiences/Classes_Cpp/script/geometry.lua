-- geometry.lua

-- Table Point (struct-like)
local Point = {}

function Point.new(x, y)
    return { x = x, y = y }
end

function Point.toStr(p)
    return "(" .. p.x .. "," .. p.y .. ")"
end

-- Meta class Line
local Line = {}
Line.__index = Line

function Line:newByPoint(p1, p2)
    local o = setmetatable({}, self)
    o.p1 = p1
    o.p2 = p2
    return o
end

function Line:newByCoordenates(x0, y0, x1, y1)
    return self:newByPoint(Point.new(x0, y0), Point.new(x1, y1))
end

function Line:toStr()
    return Point.toStr(self.p1) .. " - " .. Point.toStr(self.p2)
end

-- Other functions
local function distance(x0, y0, x1, y1)
    if type(x0) ~= "number" or type(y0) ~= "number" or type(x1) ~= "number" or type(y1) ~= "number" then
        return false, nil
    end
    return true, math.sqrt((x0 - x1) ^ 2 + (y0 - y1) ^ 2)
end

local function intersection(line1, line2)
    if getmetatable(line1) ~= Line or getmetatable(line2) ~= Line then
        return nil, nil
    end

    -- print("DEBUG: line1.p1", line1.p1 and Point.toStr(line1.p1))
    -- print("DEBUG: line1.p2", line1.p2 and Point.toStr(line1.p2))

    local approxEqual = function(a, b, eps)
        eps = eps or 1e-9
        return math.abs(a - b) < eps
    end

    local x0, y0 = line1.p1.x, line1.p1.y
    local x1, y1 = line1.p2.x, line1.p2.y
    local x2, y2 = line2.p1.x, line2.p1.y
    local x3, y3 = line2.p2.x, line2.p2.y

    local d = (y3 - y2) * (x1 - x0) - (x3 - x2) * (y1 - y0);
    local a = (x3 - x2) * (y0 - y2) - (y3 - y2) * (x0 - x2);
    local b = (x1 - x0) * (y0 - y2) - (y1 - y0) * (x0 - x2);

    if d == 0 then
        if a == 0 and b == 0 then
            return "Coincident", Point.new(x0, y0).toStr()
        end
        return "Parallel", "There are no intersection points"
    end

    local t = a / d
    local u = b / d

    if t >= 0.0 and t <= 1.0 and u >= 0.0 and u <= 1.0 then
        local p = Point.new(x0 + t * (x1 - x0), y0 + t * (y1 - y0))
        -- Check if the intersection point belongs to the two lines
        local result_1, d1 = distance(p.x, p.y, x0, y0)
        local result_2, d2 = distance(p.x, p.y, x1, y1)
        local result_3, d3 = distance(x0, y0, x1, y1)
        local result_4, d4 = distance(p.x, p.y, x2, y2)
        local result_5, d5 = distance(p.x, p.y, x3, y3)
        local result_6, d6 = distance(x2, y2, x3, y3)
        if result_1 and result_2 and result_3 and result_4 and result_5 and result_6 then
            if approxEqual(d1 + d2, d3) and approxEqual(d4 + d5, d6) then
                return "True intersection", Point.toStr(p)
            end
        end
        return "Intersection outside the line segment", Point.toStr(p)
    end
end

-- For integration with C++
function Intersection(x0, y0, x1, y1, x2, y2, x3, y3)
    local line1 = Line:newByCoordenates(x0, y0, x1, y1)
    local line2 = Line:newByCoordenates(x2, y2, x3, y3)
    local result, point = intersection(line1, line2)
    return result, point
end

-- Test in Lua interpreter.
local function test()
    local result, point = Intersection(1, 1, 5, 5, 1, 5, 5, 1)
    print(result)
    print(point)
end

-- Leave commented if used in C++ integration.
-- test()
