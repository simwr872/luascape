--client.move(275,565)
--client.click()
function find_bank()
	for y=0,300,50 do
		for x=0,500,50 do
			client.move(x,y)
			if (string.find(client.hover(), "Bank") ~= nil) then
				return
			end
		end
	end
end

find_bank()
