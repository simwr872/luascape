function dec2rgb(dec)
	local r = (dec >> 16) & 0xff
	local g = (dec >> 8) & 0xff
	local b = dec & 0xff
	return r, g, b
end

g = client.capture(0,0,1,2)
r,g,b = dec2rgb(g[1][3])
print(r, g, b)