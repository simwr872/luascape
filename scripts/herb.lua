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

function move(x, y)
	client.move(x+math.random(-5,5), y+math.random(-5,5))
end
function click(x, y)
	move(x, y)
	client.click()
end


inventory = {
	slot = {
		{{584,291,619,322},{584,327,619,358},{584,363,619,394},{584,399,619,430},{584,435,619,466},{584,471,619,502},{584,507,619,538}},
		{{633,291,668,322},{633,327,668,358},{633,363,668,394},{633,399,668,430},{633,435,668,466},{633,471,668,502},{633,507,668,538}},
		{{682,291,717,322},{682,327,717,358},{682,363,717,394},{682,399,717,430},{682,435,717,466},{682,471,717,502},{682,507,717,538}},
		{{731,291,766,322},{731,327,766,358},{731,363,766,394},{731,399,766,430},{731,435,766,466},{731,471,766,502},{731,507,766,538}}
	}
}

setup_bank()
while true do
	client.click()
	while not is_bank_open() do end
	randsleep()
	client.press("1")
	while is_bank_open() do end
	randsleep()
	click((inventory.slot[1][4][1] + inventory.slot[1][4][3])/2, (inventory.slot[1][4][2] + inventory.slot[1][4][4])/2)
	randsleep()
	click((inventory.slot[1][5][1] + inventory.slot[1][5][3])/2, (inventory.slot[1][5][2] + inventory.slot[1][5][4])/2)
	while not is_fletch_open() do end
	randsleep()
	client.press(" ")
	randsleep()
	setup_bank()
	while not is_fletch_done() do end
	randsleep()
end