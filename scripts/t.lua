function dec2rgb(dec)
	local r = (dec >> 16) & 0xff
	local g = (dec >> 8) & 0xff
	local b = dec & 0xff
	return r, g, b
end
function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end

client.move(280,280,4)
while true do
	sleep(6)
	if string.find(client.hover(), "Tune") then
		sleep(math.random(24,35))
		client.click()
	end
end
