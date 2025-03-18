// lua_libuv.c
#include "lua_libuv.h"
#include "lauxlib.h"
#include "uv.h"

static void my_timer_callback(uv_timer_t *handle) {
    lua_State *L = (lua_State *)handle->data;
    lua_getglobal(L, "on_timer");
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            fprintf(stderr, "Timer callback error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    } else {
        lua_pop(L, 1);
    }
}

static int lua_uv_start_timer(lua_State *L) {
    double timeout = luaL_checknumber(L, 1); // Timeout in milliseconds
    uv_loop_t *loop = uv_default_loop();
    uv_timer_t *timer = malloc(sizeof(uv_timer_t));
    
    uv_timer_init(loop, timer);
    timer->data = L; // Store Lua state
    uv_timer_start(timer, my_timer_callback, (uint64_t)timeout, 0); // One-shot timer
    
    lua_pushlightuserdata(L, timer); // Return timer handle
    return 1;
}

static const luaL_Reg libuv_funcs[] = {
    {"start_timer", lua_uv_start_timer},
    {NULL, NULL}
};

int luaopen_libuv(lua_State *L) {
    luaL_newlib(L, libuv_funcs);
    return 1;
}