local socket = require("socket.core")
local lfs = require("lfs")

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

function on_timer()
    print("Timer fired after 1 second!")
end

start_timer(1000)

return "Test completed"