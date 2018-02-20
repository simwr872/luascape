function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end
function dec2rgb(dec)
	local r = (dec >> 16) & 0xff
	local g = (dec >> 8) & 0xff
	local b = dec & 0xff
	return r, g, b
end

function moving() 
	r,g,b = dec2rgb(client.capture(0,0,1,1)[1][1])
	return r == g and g == b and b == 0
end

function areamove(t)
	client.move(math.random(t[1], t[3]), math.random(t[2], t[4]), t[3] - t[1])
end

function hover(t)
	return string.find(client.hover(), t[5])
end

walkway = {345,5,400,40,"Leap"}
cliff = {174,82,255,176,"Traverse"}
--window = {339,227,399,254,"Leap"}
cathedral = {183,121,272,176,"Scale"}
roof = {238,135,347,242,"Vault"}
zipline = {298,199,345,236,"Slide"}
light = {240,195,247,205,"Merge"}

steps = {walkway, cliff, cathedral, roof, zipline, light}
i = 1
while true do
	if i > 6 then
		i = 1
	end
	areamove(steps[i])
	while not hover(steps[i]) do sleep(0.2) end
	client.click()
	sleep(1.5)
	if(steps[i] == light) then
		sleep(5.3)
	end
	while moving() do sleep(0.5) end
	sleep(math.random(200,400)/1000)
	i = i + 1
end
