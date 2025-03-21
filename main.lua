local socket = require("socket.core")
local lfs = require("lfs")
local cjson = require("cjson")
local libuv = require("libuv")

local client = socket.tcp()
local success, err = client:connect("google.com", 80)
if success then
    print("Connected to google.com:80")
    client:close()
else
    print("Connection failed: " .. err)
end

print("Listing files in current directory:")
for file in lfs.dir(".") do
    if file ~= "." and file ~= ".." then
        print("  " .. file)
    end
end

local json_str = '{"name": "Alice", "age": 30}'
local parsed, err = cjson.parse(json_str)
if parsed then
    print("Parsed JSON:")
    for k, v in pairs(parsed) do
        print("  " .. k .. ": " .. tostring(v))
    end
else
    print("JSON parse error: " .. err)
end

local data = {name = "Bob", score = 42}
local json_out = cjson.stringify(data)
print("Stringified JSON: " .. json_out)

function on_timer()
    print("Timer fired after 1 second!")
end

libuv.start_timer(1000)

return "Test completed"