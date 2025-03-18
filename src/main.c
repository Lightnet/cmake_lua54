// main.c
#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "uv.h"
#include "mimalloc.h"

// Module declarations
extern int luaopen_socket_core(lua_State *L);
extern int luaopen_lfs(lua_State *L);
extern int luaopen_cjson(lua_State *L);
extern int luaopen_libuv(lua_State *L);

// Custom allocator using mimalloc
static void *mimalloc_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud; // Unused user data
    if (nsize == 0) {
        mi_free(ptr);
        return NULL;
    } else if (ptr == NULL) {
        return mi_malloc(nsize);
    } else {
        return mi_realloc(ptr, nsize);
    }
}

static void print_error(lua_State *L, const char *msg) {
    fprintf(stderr, "Error: %s: %s\n", msg, lua_tostring(L, -1));
    lua_pop(L, 1);
}

int main(int argc, char *argv[]) {
    // Initialize Lua with mimalloc allocator
    lua_State *L = lua_newstate(mimalloc_lua_alloc, NULL);
    if (L == NULL) {
        fprintf(stderr, "Fatal Error: Failed to create Lua state\n");
        return 1;
    }

    luaL_openlibs(L);

    // Register LuaSocket
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_socket_core);
    lua_setfield(L, -2, "socket.core");
    lua_pop(L, 2);

    // Register LuaFileSystem
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_lfs);
    lua_setfield(L, -2, "lfs");
    lua_pop(L, 2);

    // Register cJSON
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_cjson);
    lua_setfield(L, -2, "cjson");
    lua_pop(L, 2);

    // Register libuv
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_libuv);
    lua_setfield(L, -2, "libuv");
    lua_pop(L, 2);

    const char *lua_file = "main.lua";
    if (argc > 1) {
        const char *ext = strrchr(argv[1], '.');
        if (ext && strcmp(ext, ".lua") == 0) {
            lua_file = argv[1];
        } else {
            printf("Warning: '%s' does not have a .lua extension\n", argv[1]);
        }
    }

    int load_result = luaL_loadfile(L, lua_file);
    if (load_result != LUA_OK) {
        if (load_result == LUA_ERRFILE) {
            fprintf(stderr, "Error: Could not open or read file '%s'\n", lua_file);
        } else if (load_result == LUA_ERRSYNTAX) {
            print_error(L, "Syntax error in Lua file");
        } else if (load_result == LUA_ERRMEM) {
            fprintf(stderr, "Error: Memory allocation failed while loading '%s'\n", lua_file);
        }
        lua_close(L);
        return 1;
    }

    int run_result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (run_result != LUA_OK) {
        print_error(L, "Runtime error");
        lua_close(L);
        return 1;
    }

    // Run the libuv event loop
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    lua_close(L);
    return 0;
}