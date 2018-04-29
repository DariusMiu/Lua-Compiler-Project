func()
	n = - 0 1
	m = 1
	repeat
		temp = + n m
		n = m
		m = temp
		if >= temp 0 then print(temp) end
	until < m 0
end