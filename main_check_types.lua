local point = require("point")
local p = point.new(10, 20)

-- Define a check_type function in Lua
function check_type(value)
    local mt = getmetatable(value)
    if mt and mt.__type then
        return mt.__type
    end
    return type(value)
end

-- Check the type of p
print(point.check_type(p))     -- Output: "Point"
print(point.check_type(42))    -- Output: "number"
print(point.check_type("hi"))  -- Output: "string"
print(point.check_type({}))    -- Output: "table"

print(check_type(p))      -- Output: "Point"
print(check_type(42))     -- Output: "number"
print(check_type("hi"))   -- Output: "string"
print(check_type({}))     -- Output: "table"