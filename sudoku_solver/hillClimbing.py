from sudoku import *
import copy

def hillClimbing(m, total_states, solution, filename=None):
	current_state = Sudoku(m)

	if(filename is not None):
		current_state.createConfiguration(filename)
	else:
		current_state.createConfiguration()
	current_state.fitness()

	best_state = copy.deepcopy(current_state) 
	
	#The positions that can be analyzed
	positions = [x for x in range(0, m*m) if x not in current_state.fixed_positions]

	while(total_states > 0):
		possible_positions = positions[:]

		while(possible_positions):
			position = random.choice(possible_positions) #Pick first node
			possible_positions.remove(position)
			
			original_value = current_state.configuration[position]
			original_local_fit = current_state.fitness((position, original_value)) #Check node local fitness

			best_value = original_value
			best_local_fit = original_local_fit

			#Try every possible value for the node, picking the one with best fitness
			for value in range(1, m+1):
				new_fit = current_state.fitness((position, value)) 
				if(new_fit > best_local_fit):
					best_local_fit = new_fit
					best_value = value

			#print("Best_local_fit: {}\nOriginal_local_fit: {}".format(best_local_fit, original_local_fit))

			#If the fitness of the best one is bigger than the original, changes it
			if(best_local_fit > original_local_fit):
				current_state.configuration[position] = best_value
				current_state.fitness()

				possible_positions = positions[:]

				#Saves the best configuration
				if(best_state.fit < current_state.fit):
					best_state = copy.deepcopy(current_state)
					print(best_state)
					print("Fitness: {}".format(best_state.fit))
					if(best_state.fit == solution):
						return best_state

		if(filename is not None):
			current_state.createConfiguration(filename)
		else:
			current_state.createConfiguration()
		current_state.fitness()
		
		total_states = total_states - 1
	return best_state

def main():
	print("Size of the Sudoku (must be a square number):")
	m = int(input())
	print("Total of tries (the bigger the number, the slower will be):")
	total_states = int(input())
	print("Name of the file (N if there isn't):")
	filename = input()

	solution = 3*m*m

	if(filename == "N"):
		best_state = hillClimbing(m, total_states, solution)
	else:
		best_state = hillClimbing(m, total_states, solution, filename)

	print(best_state)
	print("Fitness: {}".format(best_state.fit))

main()
