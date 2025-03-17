#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main(int argc, char *argv[]) {
    // Create a new Lua state
    lua_State *L = luaL_newstate();
    if (L == NULL) {
        printf("Failed to create Lua state\n");
        return 1;
    }

    // Open standard Lua libraries
    luaL_openlibs(L);

    // Determine which Lua file to run
    const char *lua_file = "main.lua"; // Default file
    if (argc > 1) {
        // Check if the argument has a .lua extension
        const char *ext = strrchr(argv[1], '.');
        if (ext && strcmp(ext, ".lua") == 0) {
            lua_file = argv[1];
        } else {
            printf("Warning: '%s' does not have a .lua extension, using default 'main.lua'\n", argv[1]);
        }
    }

    // Load and run the Lua file
    if (luaL_dofile(L, lua_file) != LUA_OK) {
        printf("Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from stack
    }

    // Clean up
    lua_close(L);
    return 0;
}