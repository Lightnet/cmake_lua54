Recommended C Libraries for Basic Use

1. stb Libraries (Single-Header Libraries)

- Author: Sean Barrett
    
- Purpose: A collection of single-header libraries for various tasks.
    
- Key Options:
    
    - stb_sprintf: Lightweight sprintf replacement for string formatting.
        
    - stb_ds: Dynamic arrays, hash maps, and string buffers.
        
    - stb_truetype: Font rendering (if you need text output).
        
- Why Use It: No build system required—just include the header and define an implementation macro.
    
- Integration: Add to src/ and use directly in main.c or new modules.
    
- Example:
    
    c
    
    ```c
    #define STB_SPRINTF_IMPLEMENTATION
    #include "stb_sprintf.h"
    char buf[256];
    stbsp_sprintf(buf, "Hello, %s! %d", "World", 42);
    printf("%s\n", buf); // Hello, World! 42
    ```
    

2. klib

- Repo: [https://github.com/attractivechaos/klib](https://github.com/attractivechaos/klib)
    
- Purpose: A collection of small, efficient data structures and utilities.
    
- Key Features:
    
    - Hash tables (khash.h).
        
    - Dynamic arrays (kvec.h).
        
    - String utilities (kstring.h).
        
- Why Use It: Lightweight, header-only, and optimized for performance.
    
- Integration: Include in src/ and use in C code or expose to Lua.
    
- Example:
    
    c
    
    ```c
    #include "kvec.h"
    kvec_t(int) array;
    kv_init(array);
    kv_push(int, array, 42);
    printf("%d\n", kv_A(array, 0)); // 42
    kv_destroy(array);
    ```
    

3. linenoise

- Repo: [https://github.com/antirez/linenoise](https://github.com/antirez/linenoise)
    
- Purpose: Simple line-editing library (like a mini readline).
    
- Why Use It: Adds interactive input with history and editing to your app.
    
- Integration: Build as a static lib or include directly; call from main.c.
    
- Example:
    
    c
    
    ```c
    #include "linenoise.h"
    char *line = linenoise("prompt> ");
    if (line) {
        printf("You typed: %s\n", line);
        linenoiseFree(line);
    }
    ```
    

4. miniz

- Repo: [https://github.com/richgel999/miniz](https://github.com/richgel999/miniz)
    
- Purpose: Compression/decompression (zlib-compatible).
    
- Why Use It: Add zip file support or compress data in your app.
    
- Integration: Single file (miniz.c), build as a static lib, expose to Lua if needed.
    
- Example:
    
    c
    
    ```c
    #include "miniz.h"
    char data[] = "Hello, world!";
    mz_ulong compressed_len = 128;
    unsigned char compressed[128];
    mz_compress(compressed, &compressed_len, (unsigned char*)data, strlen(data));
    ```
    

5. argparse (e.g., argh)

- Repo: [https://github.com/adishavit/argh](https://github.com/adishavit/argh) (or similar)
    
- Purpose: Command-line argument parsing.
    
- Why Use It: Simplify handling argc/argv in main.c.
    
- Integration: Header-only, include in src/.
    
- Example:
    
    c
    
    ```c
    #include "argh.h"
    int main(int argc, char *argv[]) {
        argh::parser cmdline(argc, argv);
        if (cmdline["-v"]) printf("Verbose mode\n");
        return 0;
    }
    ```
    

6. spdlog

- Repo: [https://github.com/gabime/spdlog](https://github.com/gabime/spdlog)
    
- Purpose: Fast, header-only or static logging library.
    
- Why Use It: Add logging to your app for debugging or output.
    
- Integration: Fetch via CMake, link to lua_app.
    
- Example:
    
    c
    
    ```c
    #include "spdlog/spdlog.h"
    spdlog::info("Hello, {}!", "world"); // Hello, world!
    ```
    

7. uthash

- Repo: [https://github.com/troydhanson/uthash](https://github.com/troydhanson/uthash)
    
- Purpose: Hash table implementation (header-only).
    
- Why Use It: Simple key-value storage in C.
    
- Integration: Include in src/.
    
- Example:
    
    c
    
    ```c
    #include "uthash.h"
    struct my_struct {
        int id;            /* key */
        char name[10];
        UT_hash_handle hh; /* makes this structure hashable */
    };
    struct my_struct *users = NULL;
    struct my_struct s = {1, "Alice"};
    HASH_ADD_INT(users, id, &s);
    ```
    

---

How to Add to Your Project

General Steps

1. Fetch Library:
    
    - For header-only libs (e.g., stb, klib), copy to src/ or a new include/ folder.
        
    - For others (e.g., spdlog, miniz), use FetchContent in CMakeLists.txt.
        
2. Update CMake:
    
    - Add source files to add_executable or create a static library and link it.
        
    - Update target_include_directories.
        
3. Use in C:
    
    - Include in main.c or a new module (e.g., lua_mylib.c).
        
4. Expose to Lua (Optional):
    
    - Write a luaopen_mylib function and register it.
        

Example: Adding stb_sprintf

1. Download: Get stb_sprintf.h from [https://github.com/nothings/stb](https://github.com/nothings/stb).
    
2. Place: Put it in src/.
    
3. Update CMakeLists.txt:
    
    cmake
    
    ```cmake
    add_executable(lua_app 
        src/main.c
        src/lua_cjson.c
        src/lua_libuv.c
        # stb_sprintf.h is header-only, no need to list
    )
    target_include_directories(lua_app PRIVATE "${CMAKE_SOURCE_DIR}/src")
    ```
    
4. Use in main.c:
    
    c
    
    ```c
    #define STB_SPRINTF_IMPLEMENTATION
    #include "stb_sprintf.h"
    char buf[256];
    stbsp_sprintf(buf, "Lua version: %s", LUA_VERSION);
    printf("%s\n", buf);
    ```
    

Example: Adding spdlog via FetchContent

1. Update CMakeLists.txt:
    
    cmake
    
    ```cmake
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
    )
    message(STATUS "Fetching spdlog v1.14.1 from GitHub...")
    FetchContent_MakeAvailable(spdlog)
    
    add_executable(lua_app 
        src/main.c
        src/lua_cjson.c
        src/lua_libuv.c
    )
    target_include_directories(lua_app PRIVATE "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(lua_app PRIVATE lualib luasocketlib uvlib lfslib cjsonlib mimalloclib spdlog::spdlog)
    ```
    
2. Use in main.c:
    
    c
    
    ```c
    #include "spdlog/spdlog.h"
    spdlog::info("Starting lua_app...");
    ```
    

---

Which to Choose?

- String Formatting: stb_sprintf (simple, no dependencies).
    
- Data Structures: klib or uthash (lightweight, flexible).
    
- Input: linenoise (interactive CLI).
    
- Compression: miniz (if you need zip support).
    
- Logging: spdlog (robust, configurable).
    
- Args: argh (easy CLI parsing).