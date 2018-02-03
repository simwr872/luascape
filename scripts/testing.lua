function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end

while true do
	client.press("a")
	--sleep(2)
end