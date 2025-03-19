
# Overview of Metatables and Metamethods

- Metatable: A table assigned to another table or userdata using setmetatable (Lua) or lua_setmetatable (C). It holds metamethods that override default behavior.
- Metamethod: A function stored in the metatable under a special key (e.g., __tostring, __add) that Lua invokes for operations like addition, indexing, or string conversion.

Metamethods are particularly powerful when working with userdata (like your Point struct) in C, as they allow you to define custom behavior in Lua.

---

List of Standard Metamethods

Here’s a comprehensive list of metamethods supported by Lua (as of Lua 5.4), with detailed explanations and examples tailored to your point module.

1. __index

- Purpose: Called when accessing a nonexistent key in a table or userdata.
- Signature: function(table, key) or a table to look up keys.
- Use Case: Provides methods or default values.
- In Your Code: Already used to enable p:get_x() by setting mt.__index = mt.
    
Example in C (Already in point.c):

c
```c
lua_pushstring(L, "__index");
lua_pushvalue(L, -2);  // mt itself
lua_settable(L, -3);   // mt.__index = mt
```

Lua Example:

lua
```lua
local mt = {
    __index = function(t, key)
        return "Unknown key: " .. key
    end
}
local t = setmetatable({}, mt)
print(t.foo)  -- "Unknown key: foo"
```

---

2. __newindex
- Purpose: Called when assigning a value to a nonexistent key.
- Signature: function(table, key, value)
- Use Case: Control how new fields are added or prevent modification.
    
C Example for point:

c
```c
static int l_newindex(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "x") == 0) {
        p->x = luaL_checkinteger(L, 3);
    } else if (strcmp(key, "y") == 0) {
        p->y = luaL_checkinteger(L, 3);
    } else {
        luaL_error(L, "Cannot set field '%s' on Point", key);
    }
    return 0;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__newindex", l_newindex},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local point = require("point")
local p = point.new(10, 20)
p.x = 30        -- Works (sets p->x)
print(p:get_x()) -- 30
p.z = 5         -- Error: "Cannot set field 'z' on Point"
```

---

3. __tostring
- Purpose: Defines how a table/userdata is converted to a string (e.g., for print).
- Signature: function(table) -> string
- Use Case: Custom string representation.
    
C Example for point:

c
```c
static int l_tostring(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    lua_pushfstring(L, "Point(%d, %d)", p->x, p->y);
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__tostring", l_tostring},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local point = require("point")
local p = point.new(10, 20)
print(p)  -- "Point(10, 20)"
```

---

4. __add
- Purpose: Defines the + operator behavior.
- Signature: function(a, b) -> result
- Use Case: Arithmetic operations.

C Example for point:

c
```c
static int l_add(lua_State *L) {
    Point *p1 = (Point *)luaL_checkudata(L, 1, "Point");
    Point *p2 = (Point *)luaL_checkudata(L, 2, "Point");
    Point *result = (Point *)lua_newuserdata(L, sizeof(Point));
    result->x = p1->x + p2->x;
    result->y = p1->y + p2->y;
    luaL_getmetatable(L, "Point");
    lua_setmetatable(L, -2);
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__add", l_add},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local point = require("point")
local p1 = point.new(10, 20)
local p2 = point.new(5, 5)
local p3 = p1 + p2
print(p3:get_x())  -- 15
```

---

5. __sub, __mul, __div, __mod, __pow, __unm
- Purpose: Define subtraction (-), multiplication (*), division (/), modulo (%), exponentiation (^), and unary minus (-x).
- Signature: function(a, b) -> result (except __unm: function(a) -> result).
- Use Case: Extend arithmetic for custom types.
    
C Example for __sub:

c
```c
static int l_sub(lua_State *L) {
    Point *p1 = (Point *)luaL_checkudata(L, 1, "Point");
    Point *p2 = (Point *)luaL_checkudata(L, 2, "Point");
    Point *result = (Point *)lua_newuserdata(L, sizeof(Point));
    result->x = p1->x - p2->x;
    result->y = p1->y - p2->y;
    luaL_getmetatable(L, "Point");
    lua_setmetatable(L, -2);
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__sub", l_sub},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local p1 = point.new(10, 20)
local p2 = point.new(5, 5)
local p3 = p1 - p2
print(p3:get_x())  -- 5
```

---

6. __eq, __lt, __le

- Purpose: Define equality (==), less than (<), and less than or equal (<=).
- Signature: function(a, b) -> boolean
- Note: > and >= are derived from < and <= with operands swapped.

C Example for __eq:

c
```c
static int l_eq(lua_State *L) {
    Point *p1 = (Point *)luaL_checkudata(L, 1, "Point");
    Point *p2 = (Point *)luaL_checkudata(L, 2, "Point");
    lua_pushboolean(L, (p1->x == p2->x && p1->y == p2->y));
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__eq", l_eq},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local p1 = point.new(10, 20)
local p2 = point.new(10, 20)
print(p1 == p2)  -- true
```

---

7. __call

- Purpose: Allows a table/userdata to be called like a function.
- Signature: function(table, ...) -> result
- Use Case: Callable objects.
    
C Example for point:

c
```c
static int l_call(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    int dx = luaL_checkinteger(L, 2);
    p->x = p->x + dx;  // Shift x by argument
    return 0;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__call", l_call},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local p = point.new(10, 20)
p(5)             -- Calls p as a function, shifts x
print(p:get_x()) -- 15
```

---

8. __gc
- Purpose: Called when a userdata is garbage-collected (only for full userdata, not tables).
- Signature: function(userdata)
- Use Case: Cleanup C resources.

C Example for point:

c
```c
static int l_gc(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    printf("Cleaning up Point(%d, %d)\n", p->x, p->y);
    return 0;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__gc", l_gc},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local p = point.new(10, 20)
p = nil
collectgarbage()  -- "Cleaning up Point(10, 20)"
```

---

9. __len
- Purpose: Defines the length operator (#).
- Signature: function(table) -> number
- Use Case: Custom length for objects.

C Example for point:

c
```c
static int l_len(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    lua_pushinteger(L, 2);  // Point has 2 fields (x, y)
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__len", l_len},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local p = point.new(10, 20)
print(#p)  -- 2
```

---

10. __concat

- Purpose: Defines the concatenation operator (..).
- Signature: function(a, b) -> result
- Use Case: String or object joining.
    
C Example for point:

c
```c
static int l_concat(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    const char *str = luaL_checkstring(L, 2);
    lua_pushfstring(L, "Point(%d, %d) %s", p->x, p->y, str);
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"__concat", l_concat},  // Add this
    {NULL, NULL}
};
```

Lua Test:

lua
```lua
local p = point.new(10, 20)
print(p .. "is here")  -- "Point(10, 20) is here"
```

---

11. __pairs (Lua 5.2+)
- Purpose: Customizes the pairs() iterator.
- Signature: function(table) -> iter_func, table, initial_key
- Use Case: Custom iteration.
    
Lua Example (C is complex, typically Lua-based)**:

lua
```lua
local mt = {
    __pairs = function(t)
        return next, {x = t:get_x(), y = t:get_y()}, nil
    end
}
local p = point.new(10, 20)
setmetatable(p, mt)
for k, v in pairs(p) do
    print(k, v)  -- "x" 10, "y" 20
end
```

---

12. __ipairs (Lua 5.2+)
- Purpose: Customizes the ipairs() iterator.
- Signature: function(table) -> iter_func, table, initial_index
- Use Case: Numeric index iteration (less common for userdata).
    
---

13. __metatable (Rarely Used)
- Purpose: Protects the metatable from being accessed or changed via getmetatable/setmetatable.
- Signature: Any value (returned by getmetatable).
- Use Case: Security or immutability.
    
C Example:

c
```c
lua_pushstring(L, "Locked metatable");
lua_setfield(L, -2, "__metatable");
```

Lua Test:

lua
```lua
local p = point.new(10, 20)
print(getmetatable(p))  -- "Locked metatable"
setmetatable(p, {})     -- Error: cannot change metatable
```

---

Adding to point.c

To include these in your point.c, update point_methods and define the functions:

c
```c
static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"set_x", l_set_x},
    {"get_y", l_get_y},
    {"set_y", l_set_y},
    {"__tostring", l_tostring},
    {"__add", l_add},
    {"__gc", l_gc},
    {NULL, NULL}
};
```

Then implement each l_* function as shown above in create_point_metatable.

---

Key Points

- Full Userdata Only: Most metamethods (__gc especially) apply only to full userdata, not light userdata or tables.
- Order of Lookup: Lua checks the metatable of the first operand, then the second (for binary operators like __add).
- Flexibility: Combine metamethods to create rich object behavior.