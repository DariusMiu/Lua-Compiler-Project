func()
	a = 0
	repeat
		a = + a 1
		print (a)
		b = 0
		while < b 3 do
			b = + b 1
			print (b)
			c = 0
			repeat
				c = + c 1
				print (c)
				d = 0
				while < d 3 do
					d = + d 1
					print (d)
				end
			until == c 3
		end
	until == a 3
end