-- TEST
print("TEST")
local socket = require("socket.core")
local lfs = require("lfs")
local cjson = require("cjson")
local libuv = require("libuv")


local point = require("point")
local p = point.new(10, 20)
print(p:get_x())  -- Outputs: 10
p:set_x(30)
print(p:get_x())  -- Outputs: 30