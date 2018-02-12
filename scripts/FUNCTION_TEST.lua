fish_tile = {295,60,318,84}
bank_tile = {245,358,285,368}
bank_scan1 = {228,360,248,369}
bank_scan2 = {243,200,243,200}
fish_scan1 = {300,354,320,366}
fish_scan2 = {306,17,306,17}
function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end

function area_move(t)
	x = math.random(t[1], t[3])
	y = math.random(t[2], t[4])
	client.move(x, y)
	sleep(math.random(2,8)/10)
end

function area_click(t)
	area_move(t)
	client.click()
	sleep(math.random(2,8)/10)
end

function find(t, s)
	client.drag_find(t[1],t[2],s)
	sleep(math.random(2,8)/10)
end

function fishing()
	r,g,b = client.pixel(276,10)
	return (r+g+b)/(3*255) < 0.2
end

function full()
	r,g,b = client.pixel(745,524)
	print(r)
	return r > 90
end

function fish_wait()
	local t = os.clock()
	while not fishing() do
		if os.clock() - t > 5000 then
			break
		end
		sleep(1)
	end
end

while true do
	area_click(fish_tile)
	sleep(math.random(30,40)/10)
	while not full() do
		area_move(fish_scan1)
		find(fish_scan2, "Bait")
		client.click()
		fish_wait()
		while fishing() do sleep(3) end
	end
	area_click(bank_tile)
	sleep(math.random(30,40)/10)
	area_move(bank_scan1)
	find(bank_scan2, "Bank")
	client.click()

	sleep(math.random(30,40)/10)
	client.press("3")
	sleep(math.random(2,8)/10)
	client.press(27)
	sleep(math.random(2,8)/10)
end

