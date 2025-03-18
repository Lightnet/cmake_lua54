# Information:
  Ref build for how to build.

  Add some libraries for better handle lua script and other basic features.

# Features:
 * lua v5.4.7
 * cjson v1.7.18
 * luasocket v3.1.0
 * libuv v1.48.0
 * luafilesystem v1_8_0
 * mimalloc v2.1.7

  For reason to add libraries for easy and common libraries with reason to use them. For research and recommend from Grok 3 date 2025.

  As note if those lasted update is reason is simple. It use c build to keep it simple.

# Lua 5.4.7 Embedded Project

This project demonstrates how to build and embed Lua 5.4.7 as a static library in a C application using CMake and Visual Studio 2022. The application (lua_app.exe) runs a Lua script (main.lua) with command-line argument checking, defaulting to main.lua if no valid .lua file is provided.

Project Overview

- Goal: Build Lua 5.4.7 from source, embed it in a C program, and execute a Lua script.
- Tools: CMake, Visual Studio 2022, Git (for fetching Lua source).
- Output: A Windows executable (lua_app.exe) in build\Debug\ that runs main.lua.

Required Setup

Prerequisites

1. Visual Studio 2022:
    - Install with the "Desktop development with C++" workload.
    - Ensure the English language pack is installed (for consistent MSBuild output).
    - Path: Default is C:\Program Files\Microsoft Visual Studio\2022\Community\.
    
2. CMake:
    - Version 3.20 or higher.
    - Download from [cmake.org](https://cmake.org/download/) or install via Visual Studio’s installer.
    - Add to system PATH (e.g., C:\Program Files\CMake\bin).
        
3. Git:
    - Required for CMake’s FetchContent to download Lua from GitHub.
    - Download from [git-scm.com](https://git-scm.com/) and add to PATH (e.g., C:\Program Files\Git\bin).
        
4. Operating System:
    - Windows 10 or later (tested with Windows SDK 10.0.22621.0).
        

Directory Structure

Create a project folder (e.g., D:\devcprojects\lua54\) with these files:

```text
lua54/
├── CMakeLists.txt    # CMake configuration to fetch and build Lua
├── main.c           # C code to embed Lua and run main.lua
├── main.lua         # Lua script to execute
├── run.bat          # Batch script to build and run
└── README.md        # This file
```

Files Needed

1. CMakeLists.txt

Configures CMake to fetch Lua 5.4.7, build it as a static library, and link it to lua_app.

cmake

```cmake
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

# Build the custom executable with main.c
add_executable(lua_app main.c)
target_link_libraries(lua_app PRIVATE lualib)

# Set Debug configuration
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

# Output binaries to bin/ (optional, currently outputs to build/Debug/)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
```

2. main.c

Embeds Lua and runs a Lua script, checking command-line arguments for a .lua file.

c
```c
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
        lua_pop(L, 1);
    }

    // Clean up
    lua_close(L);
    return 0;
}
```

3. main.lua

A sample Lua script to test basic functionality and types.

lua
```lua
print("Running main.lua from embedded Lua v5.4.7!")

-- Test Lua native types
local function test_types()
    print("Type of nil:", type(nil))
    print("Type of boolean:", type(true))
    print("Type of number:", type(42))
    print("Type of string:", type("Hello"))
    print("Type of table:", type({x = 1}))
    print("Type of function:", type(function() end))
    print("Type of thread:", type(coroutine.create(function() end)))
end

test_types()
```

4. run.bat

Builds the project, copies main.lua to build\Debug\, and runs lua_app.exe.

bat
```text
@echo off
echo Setting up Visual Studio 2022 environment...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
echo Configuring and building with CMake...
if not exist build mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
echo Copying current main.lua to build\Debug folder...
copy ..\main.lua Debug\main.lua
echo Running lua_app with main.lua...
cd Debug
lua_app.exe main.lua
pause
```

How to Build and Run

1. Prepare the Environment:
    - Ensure Visual Studio 2022, CMake, and Git are installed and in your PATH.
    - Adjust the run.bat path to vcvarsall.bat if your VS2022 installation differs (e.g., Professional edition).
        
2. Set Up the Project:
    - Create a directory (e.g., D:\devcprojects\lua54\).
    - Place the above files (CMakeLists.txt, main.c, main.lua, run.bat, README.md) in it.
        
3. Build and Run:
    - Open a command prompt in the project directory.
    - Execute:
        ```text
        run.bat
        ```
        
    - This will:
        - Configure CMake to fetch Lua 5.4.7 from https://github.com/lua/lua.git.
        - Build the static library (lualib.lib) and executable (lua_app.exe).
        - Copy main.lua to build\Debug\.
        - Run lua_app.exe main.lua.
            
4. Expected Output:
    
    ```text
    Setting up Visual Studio 2022 environment...
    [vcvarsall.bat] Environment initialized for: 'x64'
    Configuring and building with CMake...
    -- Fetching Lua v5.4.7 from GitHub...
    -- Lua source directory: D:/devcprojects/lua54/build/_deps/lua-src
    [Build output...]
    Copying current main.lua to build\Debug folder...
    1 file(s) copied.
    Running lua_app with main.lua...
    Running main.lua from embedded Lua v5.4.7!
    Type of nil: nil
    Type of boolean: boolean
    Type of number: number
    Type of string: string
    Type of table: table
    Type of function: function
    Type of thread: thread
    Press any key to continue . . .
    ```
    

How It Works

CMake Configuration
- FetchContent: Downloads Lua 5.4.7 source from GitHub using the v5.4.7 tag.
- Static Library: Compiles Lua source files into lualib.lib.
- Executable: Links main.c with lualib.lib to create lua_app.exe.
    
C Code (main.c)

- Creates a Lua state (luaL_newstate()).
- Opens standard Lua libraries (luaL_openlibs()).
- Checks command-line arguments for a .lua file, defaults to main.lua.
- Runs the script with luaL_dofile().
    

Lua Script (main.lua)

- Prints a message and tests Lua’s built-in types using the type() function.
    

Batch Script (run.bat)

- Sets up the VS2022 environment with vcvarsall.bat.
- Runs CMake to configure and build the project.
- Copies main.lua to the executable’s directory (build\Debug\).
- Executes lua_app.exe with main.lua.
    

Troubleshooting

- CMake Fails: Ensure Git and CMake are in PATH. Check network access to GitHub.
- Build Errors: Verify all Lua source files are listed in CMakeLists.txt. Missing files (e.g., lutf8lib.c) cause linker errors like unresolved external symbol luaopen_utf8.
- Path Issues: If lua_app.exe isn’t found, confirm it’s in build\Debug\. Adjust run.bat if the output directory changes.
- Clean Build: Delete build\ (rmdir /s /q build) and rerun run.bat if issues persist.
    
Notes

- Lua 5.4.7 doesn’t include FFI (Foreign Function Interface). For FFI support, switch to LuaJIT (e.g., https://github.com/LuaJIT/LuaJIT.git, tag v2.1.0-beta3).
- The executable is built in Debug mode for easier debugging.


In the context of your Lua 5.4.7 embedded project, when building Lua as a static library (lualib) using the CMakeLists.txt from your setup, not all files from the Lua 5.4.7 repository are compiled into the library. The Lua repository (https://github.com/lua/lua/tree/v5.4.7) contains several files, but only a subset is needed for the core Lua library. The files that are not compiled into the library are typically those intended for building the standalone Lua interpreter (lua.c), the Lua compiler (luac.c), or documentation/test files.

Let me break this down based on the Lua 5.4.7 source from https://github.com/lua/lua/tree/v5.4.7 and your current CMakeLists.txt.

---

Files in Lua 5.4.7 Repository

The Lua 5.4.7 repository root contains the following files (as of the v5.4.7 tag):

- C source files (all in the root directory):
    
    - lapi.c, lauxlib.c, lbaselib.c, lcode.c, lcorolib.c, lctype.c, ldblib.c, ldebug.c, ldo.c, ldump.c, lfunc.c, lgc.c, linit.c, liolib.c, llex.c, lmathlib.c, lmem.c, loadlib.c, lobject.c, lopcodes.c, loslib.c, lparser.c, lstate.c, lstring.c, lstrlib.c, ltable.c, ltablib.c, ltm.c, lua.c, luac.c, lundump.c, lutf8lib.c, lvm.c, lzio.c
        
- Header files:
    
    - lapi.h, lauxlib.h, lcode.h, lctype.h, ldebug.h, ldo.h, lfunc.h, lgc.h, ljumptab.h, llex.h, llimits.h, lmem.h, lobject.h, lopcodes.h, lopnames.h, lparser.h, lprefix.h, lstate.h, lstring.h, ltable.h, ltm.h, lua.h, luaconf.h, lualib.h, lundump.h, lvm.h, lzio.h
        
- Other files:
    
    - Makefile, README, doc/* (documentation files like contents.html, manual.html, etc.)
        

Files Compiled in Your Project

Your CMakeLists.txt explicitly lists the following files in LUA_SOURCES for the lualib static library:

- lapi.c, lauxlib.c, lbaselib.c, lutf8lib.c, lcode.c, lcorolib.c, lctype.c, ldblib.c, ldebug.c, ldo.c, ldump.c, lfunc.c, lgc.c, linit.c, liolib.c, llex.c, lmathlib.c, lmem.c, loadlib.c, lobject.c, lopcodes.c, loslib.c, lparser.c, lstate.c, lstring.c, lstrlib.c, ltable.c, ltablib.c, ltm.c, lundump.c, lvm.c, lzio.c
    

These are all the core Lua library files needed to embed Lua in your application.

Files Not Compiled

Based on the Lua 5.4.7 repository and your CMakeLists.txt, the following files are not compiled into lualib:

1. lua.c:
    
    - Purpose: This is the source file for the standalone Lua interpreter (lua.exe). It contains the main() function to run Lua scripts from the command line.
        
    - Why Not Compiled: You’re embedding Lua in your own main.c, so you don’t need the standalone interpreter. Your lua_app provides its own entry point.
        
2. luac.c:
    
    - Purpose: This is the source file for the Lua compiler (luac.exe), which compiles Lua scripts into bytecode.
        
    - Why Not Compiled: Your project runs Lua scripts directly via luaL_dofile(), not precompiled bytecode, so the compiler isn’t needed.
        
3. Header Files (e.g., lua.h, luaconf.h, etc.):
    
    - Purpose: These define the Lua API and configuration options.
        
    - Why Not Compiled: Header files are not compiled directly; they’re included during compilation of the .c files. Your target_include_directories(lualib PUBLIC "${lua_SOURCE_DIR}") ensures they’re available to main.c and the library.
        
4. Non-Source Files:
    
    - Makefile: Used for building Lua manually with make, not relevant for CMake.
        
    - README: Documentation, not compiled.
        
    - doc/*: HTML and other documentation files, not part of the build.
        

Why These Files Aren’t Needed

- Embedding vs. Standalone: Your project embeds Lua as a library (lualib) within lua_app. The standalone tools (lua.c for the interpreter, luac.c for the compiler) are separate utilities not required for embedding.
    
- Minimal Footprint: By excluding lua.c and luac.c, you keep the build focused on the Lua core library, reducing unnecessary code.
    

Verification

To confirm, your CMakeLists.txt includes all 33 .c files from the Lua 5.4.7 root except lua.c and luac.c, matching the official Lua library build (see Lua’s Makefile for reference: it compiles all .c files except these two into liblua.a).

If You Need lua.c or luac.c

- For lua.c: Add it as a separate executable target in CMakeLists.txt:
    
    cmake
    
    ```cmake
    add_executable(lua_interpreter "${lua_SOURCE_DIR}/lua.c")
    target_link_libraries(lua_interpreter PRIVATE lualib)
    ```
    
    This builds lua.exe alongside lua_app.exe.
    
- For luac.c: Similarly:
    
    cmake
    
    ```cmake
    add_executable(luac "${lua_SOURCE_DIR}/luac.c")
    target_link_libraries(luac PRIVATE lualib)
    ```
    

However, your current setup doesn’t require these, as main.c handles script execution.

Updated README Snippet

Here’s an addition to your README.md to explain this:

Files Not Compiled

The following Lua 5.4.7 files are not included in the lualib static library:

- lua.c: Standalone interpreter, not needed for embedding.
- luac.c: Lua compiler, not used since scripts are run directly.
- Header files (e.g., lua.h): Included during compilation, not compiled separately.
- Non-source files (e.g., Makefile, README, doc/*): Documentation and build tools, not part of the library.

