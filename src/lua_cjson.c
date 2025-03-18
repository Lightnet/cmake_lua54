// lua_cjson.c
#include "lua_cjson.h"
#include "lauxlib.h"
#include "cJSON.h"

static int lua_cjson_parse(lua_State *L) {
    const char *json_str = luaL_checkstring(L, 1);
    cJSON *json = cJSON_Parse(json_str);
    if (!json) {
        lua_pushnil(L);
        lua_pushstring(L, cJSON_GetErrorPtr());
        return 2; // nil, error message
    }

    if (cJSON_IsObject(json)) {
        lua_newtable(L);
        cJSON *item = json->child;
        while (item) {
            lua_pushstring(L, item->string);
            if (cJSON_IsString(item)) {
                lua_pushstring(L, item->valuestring);
            } else if (cJSON_IsNumber(item)) {
                lua_pushnumber(L, item->valuedouble);
            } else {
                lua_pushnil(L); // Simplified: only strings and numbers
            }
            lua_settable(L, -3);
            item = item->next;
        }
    } else {
        lua_pushnil(L); // Simplified: only objects supported
    }

    cJSON_Delete(json);
    return 1;
}

static int lua_cjson_stringify(lua_State *L) {
    if (!lua_istable(L, 1)) {
        lua_pushnil(L);
        lua_pushstring(L, "Expected a table");
        return 2;
    }

    cJSON *json = cJSON_CreateObject();
    lua_pushnil(L); // First key
    while (lua_next(L, 1)) {
        const char *key = lua_tostring(L, -2);
        if (lua_isstring(L, -1)) {
            cJSON_AddStringToObject(json, key, lua_tostring(L, -1));
        } else if (lua_isnumber(L, -1)) {
            cJSON_AddNumberToObject(json, key, lua_tonumber(L, -1));
        }
        lua_pop(L, 1); // Pop value, keep key for next
    }

    char *json_str = cJSON_Print(json);
    lua_pushstring(L, json_str);
    cJSON_free(json_str);
    cJSON_Delete(json);
    return 1;
}

static const luaL_Reg cjson_funcs[] = {
    {"parse", lua_cjson_parse},
    {"stringify", lua_cjson_stringify},
    {NULL, NULL}
};

int luaopen_cjson(lua_State *L) {
    luaL_newlib(L, cjson_funcs);
    return 1;
}