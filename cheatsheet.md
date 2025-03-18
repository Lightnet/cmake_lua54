Cheatsheet: cJSON (Lua)

Overview

- Purpose: Parse and generate JSON in Lua using the cJSON library.
    
- Module Name: cjson (loaded via require("cjson")).
    
- Capabilities: Supports parsing JSON strings to Lua tables and stringifying Lua tables to JSON.
    

Setup in Lua

1. Ensure lua_cjson.c and lua_cjson.h are in src/.
    
2. Register in main.c:
    
    c
    
    ```c
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_cjson);
    lua_setfield(L, -2, "cjson");
    lua_pop(L, 2);
    ```
    
3. Use in Lua script:
    
    lua
    
    ```lua
    local cjson = require("cjson")
    ```
    

Functions

|Function|Parameters|Returns|Description|
|---|---|---|---|
|cjson.parse(str)|str<br><br>: JSON string|Table or<br><br>nil, error|Parses JSON string to a Lua table.|
|cjson.stringify(tbl)|tbl<br><br>: Lua table|String or<br><br>nil, error|Converts Lua table to JSON string.|

Limitations

- Only handles objects with string/number values (no arrays, nested objects, booleans yet).
    

Example Lua Script

lua

```lua
-- main.lua
local cjson = require("cjson")

-- Parsing JSON
local json_str = '{"name": "Alice", "age": 25}'
local data, err = cjson.parse(json_str)
if data then
    print("Name:", data.name)  -- Name: Alice
    print("Age:", data.age)    -- Age: 25
else
    print("Parse error:", err)
end

-- Stringifying table
local tbl = {name = "Bob", score = 42}
local json_out = cjson.stringify(tbl)
print("JSON:", json_out)  -- JSON: {"name":"Bob","score":42}
```

Visual Example

```text
Input JSON: '{"name": "Alice", "age": 25}'
       ↓ cjson.parse()
Lua Table: { name = "Alice", age = 25 }
       ↓ cjson.stringify()
Output JSON: {"name":"Alice","age":25}
```

Starter Guide

1. Create Script: Save as main.lua in project root.
    
2. Build: Run cmake .. -DCMAKE_BUILD_TYPE=Debug and cmake --build . --config Debug in build/.
    
3. Run: Execute .\bin\lua_app from build/.
    

---

Cheatsheet: libuv (C and Lua)

Overview

- Purpose: Asynchronous I/O operations (e.g., timers) in C and Lua.
    
- C API: Full libuv functionality.
    
- Lua Module: libuv (loaded via require("libuv")), currently only exposes start_timer.
    

C Setup

1. Include in main.c: #include "uv.h".
    
2. Link uvlib in CMakeLists.txt.
    

Lua Setup

1. Ensure lua_libuv.c and lua_libuv.h are in src/.
    
2. Register in main.c:
    
    c
    
    ```c
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_libuv);
    lua_setfield(L, -2, "libuv");
    lua_pop(L, 2);
    ```
    
3. Use in Lua: local libuv = require("libuv").
    

C API (Key Functions)

|Function|Parameters|Returns|Description|
|---|---|---|---|
|uv_timer_init(loop, timer)|loop<br><br>: uv_loop_t*,<br><br>timer<br><br>: uv_timer_t*|int|Initializes a timer.|
|uv_timer_start(timer, cb, timeout, repeat)|timer<br><br>: uv_timer_t*,<br><br>cb<br><br>: callback,<br><br>timeout<br><br>: uint64_t,<br><br>repeat<br><br>: uint64_t|int|Starts a timer.|
|uv_run(loop, mode)|loop<br><br>: uv_loop_t*,<br><br>mode<br><br>: uv_run_mode|int|Runs the event loop.|

Lua API

|Function|Parameters|Returns|Description|
|---|---|---|---|
|libuv.start_timer(timeout)|timeout<br><br>: number (ms)|userdata (timer handle)|Starts a one-shot timer, calls<br><br>on_timer()<br><br>.|

Example: C Timer

c

```c
#include "uv.h"
void cb(uv_timer_t* handle) { printf("Timer!\n"); }
int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, cb, 1000, 0);
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
```

Example: Lua Timer

lua

```lua
local libuv = require("libuv")
function on_timer()
    print("Timer fired!")
end
libuv.start_timer(1000)  -- Fires after 1 second
```

Visual Example

```text
Lua: libuv.start_timer(1000) → on_timer() called after 1s
C: uv_timer_start(&timer, cb, 1000, 0) → cb() called after 1s
```

Starter Guide

1. Lua: Add timer to main.lua, build, and run as above.
    
2. C: Extend lua_libuv.c with more functions (e.g., uv_fs_open) if needed.
    

---

Cheatsheet: Lua API (C Wrapper Setup)

Overview

- Purpose: Embed Lua in C, add modules, and expose globals.
    
- Key Order: Create state → Open libs → Register modules → Load/run script → Run event loop (if async) → Close state.
    

C Setup Steps

1. Create Lua State:
    
    c
    
    ```c
    lua_State *L = lua_newstate(mimalloc_lua_alloc, NULL); // With custom allocator
    // or luaL_newstate() for default allocator
    ```
    
2. Open Standard Libraries:
    
    c
    
    ```c
    luaL_openlibs(L);
    ```
    
3. Register Module:
    
    c
    
    ```c
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_mymodule);
    lua_setfield(L, -2, "mymodule");
    lua_pop(L, 2);
    ```
    
4. Set Global:
    
    c
    
    ```c
    lua_pushcfunction(L, my_function);
    lua_setglobal(L, "my_function");
    ```
    
5. Load and Run Script:
    
    c
    
    ```c
    if (luaL_loadfile(L, "main.lua") || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }
    ```
    
6. Run Event Loop (if using libuv):
    
    c
    
    ```c
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    ```
    
7. Close State:
    
    c
    
    ```c
    lua_close(L);
    ```
    

Key Lua C Functions

|Function|Parameters|Returns|Description|
|---|---|---|---|
|lua_newstate(f, ud)|f<br><br>: allocator,<br><br>ud<br><br>: user data|lua_State*|Creates a new Lua state.|
|luaL_openlibs(L)|L<br><br>: lua_State*|void|Opens standard Lua libraries.|
|lua_pushcfunction(L, f)|L<br><br>: lua_State*,<br><br>f<br><br>: C function|void|Pushes C function onto stack.|
|lua_setglobal(L, name)|L<br><br>: lua_State*,<br><br>name<br><br>: string|void|Sets top stack value as global.|
|luaL_loadfile(L, file)|L<br><br>: lua_State*,<br><br>file<br><br>: string|int|Loads Lua file.|
|lua_pcall(L, nargs, nresults, errfunc)|L<br><br>: lua_State*,<br><br>nargs<br><br>: int,<br><br>nresults<br><br>: int,<br><br>errfunc<br><br>: int|int|Calls Lua function.|

Example: Adding a Module

c

```c
// mymodule.c
#include "lua.h"
#include "lauxlib.h"
static int my_func(lua_State *L) {
    printf("Hello from C!\n");
    return 0;
}
static const luaL_Reg mymodule_funcs[] = {
    {"say_hello", my_func},
    {NULL, NULL}
};
int luaopen_mymodule(lua_State *L) {
    luaL_newlib(L, mymodule_funcs);
    return 1;
}

// main.c snippet
lua_getglobal(L, "package");
lua_getfield(L, -1, "preload");
lua_pushcfunction(L, luaopen_mymodule);
lua_setfield(L, -2, "mymodule");
lua_pop(L, 2);
```

Lua Usage

lua

```lua
local mymodule = require("mymodule")
mymodule.say_hello()  -- Hello from C!
```

Visual Example

```text
C: lua_pushcfunction(L, luaopen_mymodule) → lua_setfield(L, -2, "mymodule")
Lua: require("mymodule") → Access mymodule.say_hello()
```

Starter Guide

1. Write C Module: Create mymodule.c and mymodule.h.
    
2. Update CMake: Add src/mymodule.c to add_executable.
    
3. Register: Add registration code in main.c.
    
4. Test: Use in main.lua.
    

---

Cheatsheet: LuaSocket

Overview

- Purpose: Networking in Lua (TCP/UDP sockets).
    
- Module Name: socket.core (loaded via require("socket.core")).
    

Setup

1. Register in main.c:
    
    c
    
    ```c
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_socket_core);
    lua_setfield(L, -2, "socket.core");
    lua_pop(L, 2);
    ```
    

Key Functions

|Function|Parameters|Returns|Description|
|---|---|---|---|
|socket.tcp()|None|TCP socket|Creates a TCP socket object.|
|sock:connect(host, port)|host<br><br>: string,<br><br>port<br><br>: number|1 or nil, error|Connects to a host.|
|sock:send(data)|data<br><br>: string|bytes or nil, error|Sends data over socket.|
|sock:close()|None|None|Closes the socket.|

Example

lua

```lua
local socket = require("socket.core")
local sock = socket.tcp()
local success, err = sock:connect("google.com", 80)
if success then
    sock:send("GET / HTTP/1.0\r\n\r\n")
    print("Sent request")
    sock:close()
else
    print("Error:", err)
end
```

Visual Example

```text
socket.tcp() → sock:connect("google.com", 80) → sock:send("GET /")
```

Starter Guide

1. Add to Script: Include in main.lua.
    
2. Build and Run: As above.
    

---

Cheatsheet: LuaFileSystem

Overview

- Purpose: File system operations in Lua.
    
- Module Name: lfs (loaded via require("lfs")).
    

Setup

1. Register in main.c:
    
    c
    
    ```c
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_lfs);
    lua_setfield(L, -2, "lfs");
    lua_pop(L, 2);
    ```
    

Key Functions

|Function|Parameters|Returns|Description|
|---|---|---|---|
|lfs.dir(path)|path<br><br>: string|iterator|Iterates over directory contents.|
|lfs.attributes(path)|path<br><br>: string|table or nil, error|Gets file attributes.|

Example

lua

```lua
local lfs = require("lfs")
for file in lfs.dir(".") do
    if file ~= "." and file ~= ".." then
        print(file)
    end
end
```

Visual Example

```text
lfs.dir(".") → Iterates: "file1", "file2", ...
```

Starter Guide

1. Add to Script: Include in main.lua.
    
2. Build and Run: As above.