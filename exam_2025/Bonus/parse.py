def parse(file_path, k):
	if k == 0:
		return
		
	try:
		# Deschidem fisierul
		with open(file_path, 'r') as f:
			lines = f.readlines()
			
			if k > 0:
				lines_to_print = lines[:k]
			else:
				lines_to_print = lines[k:]
				
			for line in lines_to_print:
				print(line, end= '')

	# Evaluam si cazul in care nu se poate deschide fisierul   
	except FileNotFoundError:
		print("Can't open file!")