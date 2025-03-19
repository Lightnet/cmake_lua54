#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

// Define a simple C structure
typedef struct {
  int id;
  const char *name;
} Item;

// Function to create and push an Item as light userdata
static int l_new_item(lua_State *L) {
  int id = luaL_checkinteger(L, 1);
  const char *name = luaL_checkstring(L, 2);

  // Allocate the Item on the C side (not managed by Lua)
  Item *item = (Item *)malloc(sizeof(Item));
  item->id = id;
  item->name = name;

  // Push it as light userdata
  lua_pushlightuserdata(L, (void *)item);
  return 1;
}

// Function to get the ID from an Item (light userdata)
static int l_get_id(lua_State *L) {
  // Check if the argument is light userdata
  if (!lua_islightuserdata(L, 1)) {
      luaL_error(L, "Expected light userdata");
  }

  // Cast the light userdata back to Item*
  Item *item = (Item *)lua_touserdata(L, 1);
  lua_pushinteger(L, item->id);
  return 1;
}

// Function to get the name from an Item (light userdata)
static int l_get_name(lua_State *L) {
  if (!lua_islightuserdata(L, 1)) {
      luaL_error(L, "Expected light userdata");
  }

  Item *item = (Item *)lua_touserdata(L, 1);
  lua_pushstring(L, item->name);
  return 1;
}

// Function to free the Item (since Lua won't manage it)
static int l_free_item(lua_State *L) {
  if (!lua_islightuserdata(L, 1)) {
      luaL_error(L, "Expected light userdata");
  }

  Item *item = (Item *)lua_touserdata(L, 1);
  free(item);
  return 0;
}


static const struct luaL_Reg itemlib[] = {
    {"new", l_new_item},
    {"get_id", l_get_id},
    {"get_name", l_get_name},
    {"free", l_free_item},
    {NULL, NULL}
};

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Register the item library
    luaL_newlib(L, itemlib);
    lua_setglobal(L, "item");

    // Run a test script
    const char *script = 
        "local i = item.new(42, 'Sword')\n"
        "print('ID:', item.get_id(i))\n"
        "print('Name:', item.get_name(i))\n"
        "item.free(i)\n";

    if (luaL_dostring(L, script) != LUA_OK) {
        printf("Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    lua_close(L);
    return 0;
}