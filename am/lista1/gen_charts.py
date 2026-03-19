import matplotlib.pyplot as plt

# Configuration: Change names and title here
names = ['10', '50', '1000', 'optymalna']
title = "Średnia długość drogi"
file_path = "solution.txt"

# Read values (expects 4 space or newline separated numbers)
with open(file_path, 'r') as f:
    values = [float(val) for val in f.read().split()][:4]
    
# Create bars
bars = plt.bar(names[:len(values)], values)
    
# Add values on top
plt.bar_label(bars, padding=3)

# Create Chart
plt.bar(names, values)
plt.title(title)
plt.savefig('chart.png')