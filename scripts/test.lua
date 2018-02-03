function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end
inventory = {
	slot = {
		{{584,291,619,322},{584,327,619,358},{584,363,619,394},{584,399,619,430},{584,435,619,466},{584,471,619,502},{584,507,619,538}},
		{{633,291,668,322},{633,327,668,358},{633,363,668,394},{633,399,668,430},{633,435,668,466},{633,471,668,502},{633,507,668,538}},
		{{682,291,717,322},{682,327,717,358},{682,363,717,394},{682,399,717,430},{682,435,717,466},{682,471,717,502},{682,507,717,538}},
		{{731,291,766,322},{731,327,766,358},{731,363,766,394},{731,399,766,430},{731,435,766,466},{731,471,766,502},{731,507,766,538}}
	}
}
function move(x, y)
	client.move(x+math.random(-5,5), y+math.random(-5,5))
end
function click(x, y)
	move(x, y)
	client.click()
end
bank = {
	close = {459,15,474,30},
	preset1 = {112,373,143,400},
	preset2 = {146,373,177,400}
}
hud = {
	compass = {598,18,630,50},
	health = {595,53,527,86},
	logout = {782,0,799,20},	--clipped by home teleport
	teleport = {752,17,781,46}
}

abilitybar = {
	special = {51,339,495,362},
	slot = { -- some icons may have smaller hitbox (i.e. familiar)
		{32,376,54,405},
		{61,376,90,405},
		{97,376,126,405},
		{133,376,162,405},
		{169,376,198,405},
		{205,376,234,405},
		{241,376,270,405},
		{277,376,306,405},
		{313,376,342,405},
		{349,376,378,405},
		{385,376,414,405},
		{421,376,450,405},
		{457,376,486,405},
		{493,376,522,405}
	}
}
function didtransport()
	c = client.color(339,545)
	if c[1]+c[2]+c[3] > 600 then
		return true
	end
	return false
end
function diddeplete()
	c = client.color(312,543)
	if c[1] > 150 and c[2] < 100 then
		return true
	end
	return false
end
function porter()
	a = abilitybar.slot[13]
	click((a[3]+a[1])/2, (a[4]+a[2])/2)
end

function kg5()
move(275, 316)--bank
while true do
client.click()
move(437,108)
client.press("3")
sleep(math.random(2,6)/10)
client.click()
sleep(math.random(2,6)/10)
client.click()
sleep(math.random(2,6)/10)
client.esc()
move((inventory.slot[1][1][1] + inventory.slot[1][1][3])/2,(inventory.slot[1][1][2] + inventory.slot[1][1][4])/2)
client.rclick()
sleep(math.random(2,6)/10)
client.move((inventory.slot[1][1][1] + inventory.slot[1][1][3])/2,(inventory.slot[1][1][2] + inventory.slot[1][1][4])/2 + 60)
client.click()
sleep(math.random(10,15)/10)
client.press(" ")
move(275, 316)
sleep(math.random(20,25)/10)
end
end



move(275, 316)--bank
while true do
client.click()
move(437,108)
client.press("3")
client.rclick()
sleep(math.random(2,6)/10)
client.move(437,108+43)
client.click()
sleep(math.random(2,6)/10)
move(437,108)
client.click()
sleep(math.random(2,6)/10)
client.click()
sleep(math.random(2,6)/10)
client.esc()
move((inventory.slot[1][1][1] + inventory.slot[1][1][3])/2,(inventory.slot[1][1][2] + inventory.slot[1][1][4])/2)
client.rclick()
sleep(math.random(2,6)/10)
client.move((inventory.slot[1][1][1] + inventory.slot[1][1][3])/2,(inventory.slot[1][1][2] + inventory.slot[1][1][4])/2 + 60)
client.click()
sleep(math.random(10,15)/10)
client.press(" ")
move(275, 316)
sleep(math.random(70,75)/10)
end





--nw clockwise
--276,145
--340,109
--274,169
--219,107
function mine()
	move(276,145)
	while true do
		client.click()
		while didtransport() do end
		move(340,209)
		while not didtransport() and not diddeplete() do end
		sleep(math.random(2,6)/10)
		if diddeplete() then
			porter()
			move(340,209)
		end
		
		client.click()
		while didtransport() do end
		move(274,269)
		while not didtransport() and not diddeplete() do end
		sleep(math.random(2,6)/10)
		if diddeplete() then
			porter()
			move(274,269)
		end
		
		client.click()
		while didtransport() do end
		move(219,207)
		while not didtransport() and not diddeplete() do end
		sleep(math.random(2,6)/10)
		if diddeplete() then
			porter()
			move(219,207)
		end
		
		client.click()
		while didtransport() do end
		move(276,145)
		while not didtransport() and not diddeplete() do end
		sleep(math.random(2,6)/10)
		if diddeplete() then
			porter()
			move(276,145)
		end
	end
end

--client.press("g");

--print(inventory.slot[1][1][1])
--client.move(798,1)

--client.move(397,228)
--client.click()

--client.move(bank.preset2[1], bank.preset2[2])
--client.click()

--client.move(inventory.slot[1][2][1], inventory.slot[1][2][2])
--client.click()
--client.move(inventory.slot[2][2][1], inventory.slot[2][2][2])
--client.click()

