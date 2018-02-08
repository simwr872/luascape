function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end

function find_bank()
	client.move(244,370)
	x,y = client.drag_find(242,200,"Bank");
end

function find_fish()
	client.move(312,360)
	x,y = client.drag_find(308,0,"Bait");
end

function goto_fish()
	client.move(308,80)
	sleep(0.2)
	client.click()
	sleep(3)
end

function goto_bank()
	client.move(279,368)
	sleep(0.2)
	client.click()
	sleep(3)
end

function fish()
	find_fish()
	client.click()
end

function bank()
	goto_bank()
	find_bank()
	client.click()
	sleep(3)
	client.press("3")
	sleep(0.5)
	client.press(27) --escape
	sleep(0.2)
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

while true do
	bank()
	goto_fish()
	while not full() do
		fish()
		sleep(6)
		while fishing() do sleep(3) end
	end
end


