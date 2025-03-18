print("Running main.lua from embedded Lua v5.4.7!")

-- Test Lua native types
local function test_types()
    -- Nil
    local n = nil
    print("Type of nil:", type(n))

    -- Boolean
    local b = true
    print("Type of boolean:", type(b))

    -- Number
    local num = 42
    print("Type of number:", type(num))

    -- String
    local str = "Hello, Lua!"
    print("Type of string:", type(str))

    -- Table
    local tbl = { x = 1, y = 2 }
    print("Type of table:", type(tbl))

    -- Function
    local fn = function() return "test" end
    print("Type of function:", type(fn))

    -- Userdata (not directly creatable in pure Lua, but can check type)
    -- We'll skip this as it requires C-side interaction

    -- Thread (coroutine)
    local co = coroutine.create(function() print("coroutine") end)
    print("Type of thread:", type(co))
end

-- test_types()



