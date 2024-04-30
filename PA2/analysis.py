import matplotlib.pyplot as plt

# Read data from the text file
with open('log.txt', 'r') as file:
    lines = file.readlines()

# Extracting steps and solutions from the lines
steps = []
solutions = []
for line in lines:
    # Splitting the line based on ' ' and then extracting step and solution
    step_solution = line.strip().split(' ')
    step = int(step_solution[1])
    solution = int(step_solution[4])
    steps.append(step)
    solutions.append(solution)

# Plotting the data
plt.plot(steps, solutions, marker='o', linestyle='-')
plt.title('Relationship between Steps and Solutions')
plt.xlabel('Steps')
plt.ylabel('Best Solution')
plt.grid(True)

# Annotating the graph with final best solution and total number of steps
final_step = steps[-1]
final_solution = solutions[-1]
plt.text(final_step, final_solution, f'Final Best Solution: {final_solution}', ha='right', va='bottom')
plt.text(final_step, final_solution, f'Total Number of Steps: {final_step}', ha='right', va='top')

plt.show()



