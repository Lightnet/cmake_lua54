# Step-by-Step Setup of the point Module

1. Project Structure
- point.h: Declares the Point structure and luaopen_point.
- point.c: Implements the Lua module with userdata and metatable logic.
- Compilation: Built as a shared library (point.so) to be loaded by Lua via require.

2. point.h
c
```c
#ifndef POINT_H
#define POINT_H

#include <lua.h>

typedef struct {
    int x;
    int y;
} Point;

int luaopen_point(lua_State *L);

#endif
```

3. point.c (Core Implementation)
Here’s the full code with explanations:

c
```c
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "point.h"

static int l_new_point(lua_State *L) {
    Point *p = (Point *)lua_newuserdata(L, sizeof(Point));
    p->x = luaL_checkinteger(L, 1);
    p->y = luaL_checkinteger(L, 2);
    luaL_getmetatable(L, "Point");
    lua_setmetatable(L, -2);
    return 1;
}

static int l_get_x(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    lua_pushinteger(L, p->x);
    return 1;
}

static int l_check_type(lua_State *L) {
    if (luaL_testudata(L, 1, "Point") != NULL) {
        lua_pushstring(L, "Point");
    } else {
        lua_pushstring(L, luaL_typename(L, 1));
    }
    return 1;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"check_type", l_check_type},
    {NULL, NULL}  // Sentinel
};

static void create_point_metatable(lua_State *L) {
    luaL_newmetatable(L, "Point");          // Stack: [mt]
    luaL_setfuncs(L, point_methods, 0);     // Stack: [mt]
    lua_pushstring(L, "__index");           // Stack: [mt, "__index"]
    lua_pushvalue(L, -2);                   // Stack: [mt, "__index", mt]
    lua_settable(L, -3);                    // Stack: [mt] (mt.__index = mt)
    lua_pushstring(L, "Point");             // Stack: [mt, "Point"]
    lua_setfield(L, -2, "__type");          // Stack: [mt] (mt.__type = "Point")
    lua_pop(L, 1);                          // Stack: []
}

int luaopen_point(lua_State *L) {
    create_point_metatable(L);              // Create metatable
    luaL_newlib(L, point_methods);          // Stack: [lib]
    lua_pushcfunction(L, l_new_point);      // Stack: [lib, l_new_point]
    lua_setfield(L, -2, "new");             // Stack: [lib] (lib.new = l_new_point)
    return 1;                               // Return library table
}
```

---

Detailed Explanation of Key Components

1. point_methods
- What it is: A struct luaL_Reg array that maps Lua function names (strings) to C function pointers. It defines the methods available to the point module and its userdata.
- Structure:

  c
```c
static const struct luaL_Reg point_methods[] = {
	{"get_x", l_get_x},         // Lua: point.get_x() or p:get_x()
	{"check_type", l_check_type}, // Lua: point.check_type()
	{NULL, NULL}                // End of list
};
```
    
- Purpose: Used by luaL_setfuncs to register these functions into a table (metatable or library).
- Expanding Later: To add more methods (e.g., get_y), append new entries before the {NULL, NULL} sentinel:

c    
```c
static const struct luaL_Reg point_methods[] = {
	{"get_x", l_get_x},
	{"get_y", l_get_y},  // New method
	{"check_type", l_check_type},
	{NULL, NULL}
};
```
    

2. create_point_metatable
- What it does: Sets up the metatable for "Point" userdata, defining its behavior.
- Step-by-Step:
    
c    
```c
static void create_point_metatable(lua_State *L) {
	luaL_newmetatable(L, "Point");      // Creates or gets metatable "Point"
	luaL_setfuncs(L, point_methods, 0); // Adds methods to metatable
	lua_pushstring(L, "__index");       // Pushes key "__index"
	lua_pushvalue(L, -2);               // Copies metatable to be __index
	lua_settable(L, -3);                // Sets mt.__index = mt
	lua_pushstring(L, "Point");         // Pushes "Point" string
	lua_setfield(L, -2, "__type");      // Sets mt.__type = "Point"
	lua_pop(L, 1);                      // Removes metatable from stack
}
```

3. Key Lua C API Functions
- luaL_newmetatable(L, "Point"):
    - Creates a new metatable named "Point" in the Lua registry (or retrieves it if it exists).
    - Pushes it onto the stack.
    - Stack: [mt]
- luaL_setfuncs(L, point_methods, 0):
    - Registers all functions from point_methods into the table at the top of the stack (the metatable).
    - The 0 means no upvalues (extra arguments for closures).
    - Stack: [mt] (unchanged, but mt now has get_x, check_type, etc.)
- lua_pushstring(L, "__index"):
    - Pushes the string "__index" onto the stack.
    - Stack: [mt, "__index"]
- lua_pushvalue(L, -2):
    - Copies the value at stack index -2 (the metatable) to the top of the stack.
    - -2: Refers to the second item from the top. With [mt, "__index"], -2 is mt.
    - Stack: [mt, "__index", mt]
    - Why: Sets the metatable as its own __index, enabling method lookup (e.g., p:get_x()).
- lua_settable(L, -3):
    - Takes the key ("__index") at -2 and value (mt) at -1, setting them in the table at -3 (the original metatable).
    - Pops the key and value.
    - Result: mt.__index = mt.
    - Stack: [mt]
- lua_pushstring(L, "Point"):
    - Pushes the string "Point" onto the stack.
    - Stack: [mt, "Point"]
- lua_setfield(L, -2, "__type"):
    - Sets the field "__type" in the table at -2 (the metatable) to the value at the top ("Point").
    - Pops the value.
    - Result: mt.__type = "Point".
    - Stack: [mt]
- lua_pop(L, 1):
    - Removes 1 item (the metatable) from the stack.
    - Stack: []
- luaL_testudata(L, 1, "Point"):
    - Tests if the value at stack index 1 is a userdata with the metatable "Point".
    - Returns a void* pointer to the userdata if true, NULL if false.
    - Used in l_check_type for type checking.
- luaL_typename(L, 1):
    - Returns the Lua type name (e.g., "userdata", "number") of the value at stack index 1 as a C string.
    - Used as a fallback in l_check_type.

---

Visual Reference: How Metatables Work

Imagine the stack and metatable setup visually:

Initial Stack (After luaL_newmetatable)

```text
Stack: [mt]
```

- mt is the new metatable for "Point".

After luaL_setfuncs

```text
Stack: [mt]
mt = { get_x = l_get_x, check_type = l_check_type }
```

After __index Setup

```text
Stack: [mt, "__index", mt]  (lua_pushvalue)
      ↓ lua_settable
Stack: [mt]
mt = { get_x = l_get_x, check_type = l_check_type, __index = mt }
```

- mt.__index = mt means the metatable looks up methods in itself.
    
After __type Setup

```text
Stack: [mt, "Point"]  (lua_pushstring)
      ↓ lua_setfield
Stack: [mt]
mt = { get_x = l_get_x, check_type = l_check_type, __index = mt, __type = "Point" }
```

Final Cleanup

```text
Stack: [mt]
      ↓ lua_pop
Stack: []
```

In Lua

When p = point.new(10, 20):

- p is a userdata with metatable mt.
- p:get_x() → Lua checks p for get_x, doesn’t find it, looks in mt.__index (which is mt), finds get_x.
    
---

Expanding the Table Later

To add more methods (e.g., set_x):

1. Update point_methods:
    
c
```c
static const struct luaL_Reg point_methods[] = {
	{"get_x", l_get_x},
	{"set_x", l_set_x},  // New method
	{"check_type", l_check_type},
	{NULL, NULL}
};
```
    
2. Define the Function:
    
c    
```c
static int l_set_x(lua_State *L) {
	Point *p = (Point *)luaL_checkudata(L, 1, "Point");
	p->x = luaL_checkinteger(L, 2);
	return 0;
}
```
    
3. Recompile: The metatable will automatically include the new method via luaL_setfuncs.
    
No changes to create_point_metatable are needed because it uses the updated point_methods.

---

Stack Indices Explained

- Positive Indices: Start from the bottom (1 is the first item).
- Negative Indices: Start from the top (-1 is the top, -2 is second from top).
- Example:
```text
Stack: [A, B, C]
Indices:  1  2  3  (positive)
		 -3 -2 -1  (negative)
```
    
- lua_pushvalue(L, -2): Copies the item at -2 (second from top) to the top.
    
---

Other Useful Lua C API Functions

- lua_getmetatable(L, index): Pushes the metatable of the value at index.
- lua_setmetatable(L, index): Sets the metatable at index to the table at the top, popping it.
- lua_newtable(L): Creates a new table and pushes it.
- lua_pushcfunction(L, func): Pushes a C function onto the stack.

---

Compile and Test

bash

```bash
gcc -shared -fPIC -o point.so point.c -I/path/to/lua/include -L/path/to/lua/lib -llua
```

lua

```lua
local point = require("point")
local p = point.new(10, 20)
print(point.check_type(p))  -- "Point"
print(p:get_x())            -- 10
```

---
