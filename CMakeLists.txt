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
    "${lua_SOURCE_DIR}/lutf8lib.c"
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

# Fetch LuaSocket
FetchContent_Declare(
    luasocket
    GIT_REPOSITORY https://github.com/lunarmodules/luasocket.git
    GIT_TAG v3.1.0
)
message(STATUS "Fetching LuaSocket v3.1.0 from GitHub...")
FetchContent_MakeAvailable(luasocket)

# List LuaSocket source files
set(LUASOCKET_SOURCES
    "${luasocket_SOURCE_DIR}/src/luasocket.c"
    "${luasocket_SOURCE_DIR}/src/timeout.c"
    "${luasocket_SOURCE_DIR}/src/buffer.c"
    "${luasocket_SOURCE_DIR}/src/io.c"
    "${luasocket_SOURCE_DIR}/src/auxiliar.c"
    "${luasocket_SOURCE_DIR}/src/options.c"
    "${luasocket_SOURCE_DIR}/src/inet.c"
    "${luasocket_SOURCE_DIR}/src/tcp.c"
    "${luasocket_SOURCE_DIR}/src/udp.c"
    "${luasocket_SOURCE_DIR}/src/except.c"
    "${luasocket_SOURCE_DIR}/src/select.c"
)
if(WIN32)
    list(APPEND LUASOCKET_SOURCES "${luasocket_SOURCE_DIR}/src/wsocket.c")
    target_link_libraries(lualib PUBLIC ws2_32)
else()
    list(APPEND LUASOCKET_SOURCES "${luasocket_SOURCE_DIR}/src/usocket.c")
endif()

add_library(luasocketlib STATIC ${LUASOCKET_SOURCES})
target_include_directories(luasocketlib PUBLIC "${luasocket_SOURCE_DIR}/src")
target_link_libraries(luasocketlib PUBLIC lualib)

# Fetch libuv
FetchContent_Declare(
    libuv
    GIT_REPOSITORY https://github.com/libuv/libuv.git
    GIT_TAG v1.48.0
)
message(STATUS "Fetching libuv v1.48.0 from GitHub...")
FetchContent_MakeAvailable(libuv)

# List libuv source files (core files)
set(LIBUV_SOURCES
    "${libuv_SOURCE_DIR}/src/fs-poll.c"
    "${libuv_SOURCE_DIR}/src/idna.c"
    "${libuv_SOURCE_DIR}/src/inet.c"
    "${libuv_SOURCE_DIR}/src/random.c"
    "${libuv_SOURCE_DIR}/src/strscpy.c"
    "${libuv_SOURCE_DIR}/src/threadpool.c"
    "${libuv_SOURCE_DIR}/src/timer.c"
    "${libuv_SOURCE_DIR}/src/uv-common.c"
    "${libuv_SOURCE_DIR}/src/uv-data-getter-setters.c"
    "${libuv_SOURCE_DIR}/src/version.c"
)

# Add platform-specific libuv source files
if(WIN32)
    list(APPEND LIBUV_SOURCES
        "${libuv_SOURCE_DIR}/src/win/async.c"
        "${libuv_SOURCE_DIR}/src/win/core.c"
        "${libuv_SOURCE_DIR}/src/win/detect-wakeup.c"
        "${libuv_SOURCE_DIR}/src/win/dl.c"
        "${libuv_SOURCE_DIR}/src/win/error.c"
        "${libuv_SOURCE_DIR}/src/win/fs-event.c"
        "${libuv_SOURCE_DIR}/src/win/fs.c"
        "${libuv_SOURCE_DIR}/src/win/getaddrinfo.c"
        "${libuv_SOURCE_DIR}/src/win/getnameinfo.c"
        "${libuv_SOURCE_DIR}/src/win/handle.c"
        "${libuv_SOURCE_DIR}/src/win/loop-watcher.c"
        "${libuv_SOURCE_DIR}/src/win/pipe.c"
        "${libuv_SOURCE_DIR}/src/win/poll.c"
        "${libuv_SOURCE_DIR}/src/win/process-stdio.c"
        "${libuv_SOURCE_DIR}/src/win/process.c"
        "${libuv_SOURCE_DIR}/src/win/signal.c"
        "${libuv_SOURCE_DIR}/src/win/stream.c"
        "${libuv_SOURCE_DIR}/src/win/tcp.c"
        "${libuv_SOURCE_DIR}/src/win/thread.c"
        "${libuv_SOURCE_DIR}/src/win/tty.c"
        "${libuv_SOURCE_DIR}/src/win/udp.c"
        "${libuv_SOURCE_DIR}/src/win/util.c"
        "${libuv_SOURCE_DIR}/src/win/winapi.c"
        "${libuv_SOURCE_DIR}/src/win/winsock.c"
    )
else()
    list(APPEND LIBUV_SOURCES
        "${libuv_SOURCE_DIR}/src/unix/async.c"
        "${libuv_SOURCE_DIR}/src/unix/core.c"
        "${libuv_SOURCE_DIR}/src/unix/dl.c"
        "${libuv_SOURCE_DIR}/src/unix/fs.c"
        "${libuv_SOURCE_DIR}/src/unix/getaddrinfo.c"
        "${libuv_SOURCE_DIR}/src/unix/getnameinfo.c"
        "${libuv_SOURCE_DIR}/src/unix/loop-watcher.c"
        "${libuv_SOURCE_DIR}/src/unix/loop.c"
        "${libuv_SOURCE_DIR}/src/unix/pipe.c"
        "${libuv_SOURCE_DIR}/src/unix/poll.c"
        "${libuv_SOURCE_DIR}/src/unix/process.c"
        "${libuv_SOURCE_DIR}/src/unix/random-devurandom.c"
        "${libuv_SOURCE_DIR}/src/unix/signal.c"
        "${libuv_SOURCE_DIR}/src/unix/stream.c"
        "${libuv_SOURCE_DIR}/src/unix/tcp.c"
        "${libuv_SOURCE_DIR}/src/unix/thread.c"
        "${libuv_SOURCE_DIR}/src/unix/tty.c"
        "${libuv_SOURCE_DIR}/src/unix/udp.c"
    )
endif()

# Build libuv as a static library
add_library(uvlib STATIC ${LIBUV_SOURCES})
target_include_directories(uvlib PUBLIC 
    "${libuv_SOURCE_DIR}/include"
    "${libuv_SOURCE_DIR}/src"
)
if(WIN32)
    target_link_libraries(uvlib PUBLIC ws2_32 psapi iphlpapi userenv DbgHelp)
    target_compile_definitions(uvlib PRIVATE _WIN32_WINNT=0x0602)
endif()

# Fetch LuaFileSystem
FetchContent_Declare(
    luafilesystem
    GIT_REPOSITORY https://github.com/lunarmodules/luafilesystem.git
    GIT_TAG v1_8_0
)
message(STATUS "Fetching LuaFileSystem v1_8_0 from GitHub...")
FetchContent_MakeAvailable(luafilesystem)

# List LuaFileSystem source files
set(LFS_SOURCES
    "${luafilesystem_SOURCE_DIR}/src/lfs.c"
)

# Build LuaFileSystem as a static library
add_library(lfslib STATIC ${LFS_SOURCES})
target_include_directories(lfslib PUBLIC "${luafilesystem_SOURCE_DIR}/src")
target_link_libraries(lfslib PUBLIC lualib)

# Fetch cJSON
FetchContent_Declare(
    cjson
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG v1.7.18
)
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "") # Disable cJSON tests
set(CJSON_OverrideBuildSharedLibs ON CACHE INTERNAL "") # Force static build
message(STATUS "Fetching cJSON v1.7.18 from GitHub...")
FetchContent_MakeAvailable(cjson)

# List cJSON source files
set(CJSON_SOURCES
    "${cjson_SOURCE_DIR}/cJSON.c"
)

# Build cJSON as a static library
add_library(cjsonlib STATIC ${CJSON_SOURCES})
target_include_directories(cjsonlib PUBLIC "${cjson_SOURCE_DIR}")

# Fetch mimalloc
FetchContent_Declare(
    mimalloc
    GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG v2.1.7
)
set(MI_BUILD_TESTS OFF CACHE INTERNAL "") # Disable mimalloc tests
set(MI_BUILD_EXAMPLES OFF CACHE INTERNAL "") # Disable mimalloc examples
message(STATUS "Fetching mimalloc v2.1.7 from GitHub...")
FetchContent_MakeAvailable(mimalloc)

# List mimalloc source files
set(MIMALLOC_SOURCES
    "${mimalloc_SOURCE_DIR}/src/static.c"  # Includes all necessary mimalloc sources
)

# Build mimalloc as a static library
add_library(mimalloclib STATIC ${MIMALLOC_SOURCES})
target_include_directories(mimalloclib PUBLIC "${mimalloc_SOURCE_DIR}/include")

# Build the custom executable with main.c and new module files
add_executable(lua_app 
    src/main.c
    src/lua_cjson.c
    src/lua_libuv.c
)
target_include_directories(lua_app PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"  # For lua_cjson.h and lua_libuv.h
)
target_link_libraries(lua_app PRIVATE lualib luasocketlib uvlib lfslib cjsonlib mimalloclib)

# Set Debug configuration
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

# Output binaries to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")