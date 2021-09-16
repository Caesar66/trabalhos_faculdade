import os
import random
import math

#FITNESS FUNCTION:
#FOR CHECKLINES(), CHECKCOLUMNS(), CHECKSQUARES():
# fitness(conflicts,m) = -4/(m-1) * conflicts + m
#FOR CHECKNUMBERS()
# fitness(quantity,m) = m - |m-quantity|

class Sudoku:
	def __init__(self, m):
		self.m = m
		self.configuration = []
		self.fixed_positions = []
		self.fit = 0

	#Creates a configuration of size m with fixed positions or not.
	def createConfiguration(self, filename=None):
		if(filename is not None): #Existem numeros pre-determinados
			with open(filename, "r") as file_txt: #Aqui fazemos o tratamento do arquivo para ficar no formato de lista
				file_size = os.stat(filename).st_size
				new_configuration = file_txt.read(file_size).replace('\n','').split(',')
				for position, number in enumerate(new_configuration):
					if((number != '0') and (number is not None)):
						self.fixed_positions.append(position)
						new_configuration[position] = int(new_configuration[position])
					else:
						new_configuration[position] = random.randint(1, self.m)
		else:
			new_configuration = [0]*(self.m*self.m)
			for position, number in enumerate(new_configuration):
				if(number == 0):
					new_configuration[position] = random.randint(1, self.m)
		self.configuration = new_configuration

	#Check the counts the number os combinations by 2 of each number in the list there is
	def checkConflicts(self, number_list):
		number_marker = self.m*[0]
		conflicts = self.m*[0]

		for number in range(self.m):
			conflicts[number_list[number]-1] = conflicts[number_list[number]-1] + number_marker[number_list[number]-1]
			number_marker[number_list[number]-1] = number_marker[number_list[number]-1] + 1
		return sum(conflicts)

	#calculate the fitness of the configuration or of the specific house
	def fitness(self, house=None):
		if(house is None):
			self.fit = self.checkLines() + self.checkColumns() + self.checkSquares()# + self.checkNumbers()
			return self.fit
		else:
			return self.checkLines(house) + self.checkColumns(house) + self.checkSquares(house)# + self.checkNumbers(house)

	#Check number quantity on the board
	#def checkNumbers(self, house=None):
	#	m = self.m
	#	fit = 0
	#
	#	number_list = m*[0]
	#	for value in self.configuration:
	#		number_list[value-1] = number_list[value-1] + 1
	#	
	#	if(house is not None):
	#		number_list[house[1]-1] = number_list[house[1]-1] + 1 
	#		fit = 1 - abs(1-m/number_list[house[1]-1])
	#	else:	
	#		for number in number_list:
	#			fit = fit + m - abs(m-number/9)
	#
	#	return fit

	def checkLines(self, house=None):
		fit = 0
		m = self.m
		configuration = self.configuration[:]

		if(house is None): #Count all lines
			for line_number in range(self.m):
				line = configuration[line_number*m:(line_number+1)*m]
				fit = fit - (4/(m-1))*self.checkConflicts(line) + m
		else: #Only the line of the house
			configuration[house[0]] = house[1]
			line_number = int(house[0]/m)
			line = configuration[line_number*m:(line_number+1)*m]
			fit = fit - (4/(m-1))*self.checkConflicts(line) + m

		return fit

	def checkColumns(self, house=None):
		fit = 0
		m = self.m
		configuration = self.configuration[:]

		if(house is None): #Count all columns
			for column_number in range(m):
				column = configuration[column_number:column_number+m*(m-1)+1:m]
				fit = fit - (4/(m-1))*self.checkConflicts(column) + m
		else: #Only the column of the house
			configuration[house[0]] = house[1]
			column_number = house[0]%m
			column = configuration[column_number:column_number+m*(m-1)+1:m]				
			fit = fit - (4/(m-1))*self.checkConflicts(column) + m

		return fit

	def checkSquares(self, house=None):
		fit = 0
		m = self.m
		m_sqrt = int(math.sqrt(self.m))
		configuration = self.configuration[:]
			
		square = []
		if(house is None):
			for square_line in range(m_sqrt):
				for initial_pos in range(m_sqrt):
					for subline in range(m_sqrt):
						square = square + self.configuration[square_line*(m*m_sqrt)+initial_pos*m_sqrt+subline*m:square_line*(m*m_sqrt)+initial_pos*m_sqrt+subline*m+m_sqrt]
					fit = fit - (4/(m-1))*self.checkConflicts(square) + m
					square = []
		else:
			configuration[house[0]] = house[1]
			square_line = int(house[0]/(m*m_sqrt))
			initial_pos = int((house[0]%m)/m_sqrt)

			for subline in range(m_sqrt):
				square = square + configuration[square_line*(m*m_sqrt)+initial_pos*m_sqrt+subline*m:square_line*(m*m_sqrt)+initial_pos*m_sqrt+subline*m+m_sqrt]
			fit = fit - (4/(m-1))*self.checkConflicts(square) + m
		return fit

	#Method to format configuration in a more friendly way
	def __str__(self):
		board = ""
		separator = int(math.sqrt(self.m))
		max_digit_number = len(str(self.m))

		for line in range(self.m):
			if(line%separator == 0 and line != 0):
				board = board + ((self.m)*(max_digit_number+1))*"_" + (2*(separator-1)-1)*"_" + "\n"
			for house in range(self.m):
				if(house%separator == 0 and house != 0):
					board = board + "| "
				new_digit = str(self.configuration[line*self.m + house])
				board = board + new_digit + (max_digit_number-len(new_digit)+1)*" "
			board = board + "\n"
		return board
