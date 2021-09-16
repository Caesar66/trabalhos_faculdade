import random

#New Random Configuration
def newConfiguration(board_disposition):
	new_queens_positions = []
	for column in board_disposition:
		new_queens_positions.append(random.randint(column[0],column[1]))
	return new_queens_positions

# For horizontal queen if queen1_pos mod 8 == queen2_pos mod 8
# For diagonal queen need:
#	Check parity between |queen1_pos - queen2_pos| [I named delta]
#	Check parity of queen1_pos and queen2_pos
#	Now If parity of queen1_pos == queen2_pos, then delta%2 == 1 (Essentially, it is counting the number of column jumps it is being made between queen1_pos and queen2_pos)
#	If parity of queen1_pos != queen2_pos, then delta%2 == 2
def queenEvaluation(queen, queens_positions):
	currentQueenSolution = 0
	for q in queens_positions:
		if(q == queen):
			continue
		else:
			if(queen%8 == q%8):
				currentQueenSolution = currentQueenSolution + 1
				continue

			delta = abs((int(queen/8)+1)-(int(q/8)+1))
			par_q = q%2
			par_queen = queen%2

			if((delta%2 == 1 and par_q != par_queen) or (delta%2 == 0 and par_q == par_queen)):
				if(abs(queen-q)%7 == 0 or abs(queen-q)%9 == 0):
					currentQueenSolution = currentQueenSolution + 1
	return currentQueenSolution

#Here, the function attempts to move a queen to a new house and decrease the number of attacking queens
def attemptConfiguration(board_disposition, queens_positions, currentSolution):
	queenToMove = random.randint(0,7)
	newHouse = random.randint(board_disposition[queenToMove][0], board_disposition[queenToMove][1])

	new_board_eval = currentSolution - queenEvaluation(queens_positions[queenToMove], queens_positions) + queenEvaluation(newHouse, queens_positions)

	if(new_board_eval <= currentSolution):
		currentSolution = new_board_eval
		queens_positions[queenToMove] = newHouse
		return queens_positions, currentSolution
	else:
		return queens_positions, currentSolution

#This function gets the total of attacking queens in the current configuration
def configurationEvaluation(queens_positions):
	currentSolution = 0
	for queen in range(len(queens_positions)):
		currentEval = queenEvaluation(queens_positions[queen], queens_positions[queen:])
		currentSolution = currentSolution + currentEval

	return currentSolution

def hillClimbing(solution, tries):
	board_disposition = [[0,7],[8,15],[16,23],[24,31],[32,39],[40,47],[48,55],[56,63]]
	bestSolution = 28
	bestConfiguration = [0,8,16,24,32,40,48,56]

	for counter in range(tries):
		queens_positions = newConfiguration(board_disposition)
		currentSolution = configurationEvaluation(queens_positions)
		if(currentSolution < bestSolution):
			bestSolution = currentSolution
			bestConfiguration = queens_positions
		nextNode = True

		while(nextNode):
			queens_positions_temp, update = attemptConfiguration(board_disposition, queens_positions, currentSolution)
			#print(update, currentSolution)
			if(update <= bestSolution):
				bestSolution = update
				bestConfiguration = queens_positions_temp

				currentSolution = update
				queens_positions = queens_positions_temp
				if(currentSolution == solution):
						nextNode = False
			else:
				nextNode = False

	for pos in range(len(bestConfiguration)):
		bestConfiguration[pos] = bestConfiguration[pos] + 1
	return bestSolution, bestConfiguration
			
solution = 0
tries = 1

print(hillClimbing(solution, tries))
