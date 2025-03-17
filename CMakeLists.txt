cmake_minimum_required(VERSION 3.20)
project(lua_embedded LANGUAGES C)

# Include FetchContent module
include(FetchContent)

# Fetch Lua v5.4.7 from GitHub
FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.4.7
)
message(STATUS "Fetching Lua v5.4.7 from GitHub...")
FetchContent_MakeAvailable(lua)

# Debug: Print the fetched source directory
message(STATUS "Lua source directory: ${lua_SOURCE_DIR}")

# Verify the existence of lapi.c in the root
if(NOT EXISTS "${lua_SOURCE_DIR}/lapi.c")
    message(FATAL_ERROR "lapi.c not found in ${lua_SOURCE_DIR}/. FetchContent failed to download Lua sources.")
endif()

# Explicitly list Lua source files without src/ prefix
set(LUA_SOURCES
    "${lua_SOURCE_DIR}/lapi.c"
    "${lua_SOURCE_DIR}/lauxlib.c"
    "${lua_SOURCE_DIR}/lbaselib.c"
    "${lua_SOURCE_DIR}/lutf8lib.c"  # Required for luaopen_utf8
    "${lua_SOURCE_DIR}/lcode.c"
    "${lua_SOURCE_DIR}/lcorolib.c"
    "${lua_SOURCE_DIR}/lctype.c"
    "${lua_SOURCE_DIR}/ldblib.c"
    "${lua_SOURCE_DIR}/ldebug.c"
    "${lua_SOURCE_DIR}/ldo.c"
    "${lua_SOURCE_DIR}/ldump.c"
    "${lua_SOURCE_DIR}/lfunc.c"
    "${lua_SOURCE_DIR}/lgc.c"
    "${lua_SOURCE_DIR}/linit.c"
    "${lua_SOURCE_DIR}/liolib.c"
    "${lua_SOURCE_DIR}/llex.c"
    "${lua_SOURCE_DIR}/lmathlib.c"
    "${lua_SOURCE_DIR}/lmem.c"
    "${lua_SOURCE_DIR}/loadlib.c"
    "${lua_SOURCE_DIR}/lobject.c"
    "${lua_SOURCE_DIR}/lopcodes.c"
    "${lua_SOURCE_DIR}/loslib.c"
    "${lua_SOURCE_DIR}/lparser.c"
    "${lua_SOURCE_DIR}/lstate.c"
    "${lua_SOURCE_DIR}/lstring.c"
    "${lua_SOURCE_DIR}/lstrlib.c"
    "${lua_SOURCE_DIR}/ltable.c"
    "${lua_SOURCE_DIR}/ltablib.c"
    "${lua_SOURCE_DIR}/ltm.c"
    "${lua_SOURCE_DIR}/lundump.c"
    "${lua_SOURCE_DIR}/lvm.c"
    "${lua_SOURCE_DIR}/lzio.c"
)

# Build Lua as a static library
add_library(lualib STATIC ${LUA_SOURCES})
target_include_directories(lualib PUBLIC "${lua_SOURCE_DIR}")

# Build the custom executable with main.c
add_executable(lua_app main.c)
target_link_libraries(lua_app PRIVATE lualib)

# Set Debug configuration
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

# Output binaries to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")