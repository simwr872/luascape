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

client.move(260,260)
while true do
	client.click()
	sleep(math.random()+1)
	client.press("1")
	sleep(math.random()+1)
	client.press("1")
	sleep(math.random()+1)
	client.press(" ")
	sleep(math.random(17,18) + math.random()+1)
end
client.press("1")
while true do end

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

