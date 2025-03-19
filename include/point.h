#ifndef POINT_H
#define POINT_H

#include <lua.h>

// Define the Point structure
typedef struct {
    int x;
    int y;
} Point;

// Function to register the point library with Lua
int luaopen_point(lua_State *L);

#endif // POINT_H