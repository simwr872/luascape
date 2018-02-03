
function setup_bank()
	client.move(273+math.random(-10,10),313+math.random(-10,10));
end

function sleep(n)
	local t = os.clock()
	while os.clock() - t <= n do end
end

function randsleep(n,i)
	sleep(n + math.random(0,i*10)/10)
end


setup_bank()
while true do
	client.click()
	randsleep(1,1)
	client.press("1")
	randsleep(1,1)
	client.press("1")
	randsleep(1,1)
	client.press(" ")
	randsleep(51,1)
end