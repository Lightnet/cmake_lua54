@echo off
if not exist build mkdir build
cd build
echo Copying current main.lua to build\Debug folder...
copy ..\main.lua Debug\main.lua
echo Running lua_app with main.lua...
cd Debug
lua_app.exe main.lua
@REM pause