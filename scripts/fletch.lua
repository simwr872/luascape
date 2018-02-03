function is_bank_open()
	c = client.color(467,23)
	if c[1]+c[2]+c[3] < 150 then
		c2 = client.color(442,18)
		return c2[1] > 160 and c2[2] > 130
	end
	return false
end

function is_fletch_open()
	c = client.color(283,370)
	if c[3] > 120 then
		c2 = client.color(463,370)
		return c2[3] > 120
	end
	return false
end

function is_fletch_done()
	c = client.color(434,125)
	return c[2] > 110
end

function setup_bank()
	client.move(273+math.random(-10,10),313+math.random(-10,10));
end

function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end

function randsleep()
	sleep(math.random(5,10)/10)
end



setup_bank()
while true do
	client.click()
	while not is_bank_open() do end
	print("bank open")
	randsleep()
	client.press("1")
	while is_bank_open() do end
	randsleep()
	client.press("1")
	while not is_fletch_open() do end
	randsleep()
	client.press(" ")
	while not is_fletch_done() do end
	sleep(1)
	randsleep()
end