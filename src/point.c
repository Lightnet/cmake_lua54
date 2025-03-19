#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "point.h"

// typedef struct {
//     int x;
//     int y;
// } Point;

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

static int l_set_x(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    p->x = luaL_checkinteger(L, 2);
    return 0;
}

static int l_get_y(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    lua_pushinteger(L, p->y);
    return 1;
}

static int l_set_y(lua_State *L) {
    Point *p = (Point *)luaL_checkudata(L, 1, "Point");
    p->y = luaL_checkinteger(L, 2);
    return 0;
}

static const struct luaL_Reg point_methods[] = {
    {"get_x", l_get_x},
    {"set_x", l_set_x},
    {"get_y", l_get_y},
    {"set_y", l_set_y},
    {NULL, NULL}
};

static void create_point_metatable(lua_State *L) {
    luaL_newmetatable(L, "Point");
    luaL_setfuncs(L, point_methods, 0);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
    lua_pop(L, 1);
}

int luaopen_point(lua_State *L) {
    create_point_metatable(L);
    luaL_newlib(L, point_methods);
    lua_pushcfunction(L, l_new_point);
    lua_setfield(L, -2, "new");
    return 1;
}