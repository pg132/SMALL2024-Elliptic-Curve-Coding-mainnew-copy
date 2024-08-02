#import csv
#import itertools
import matplotlib.pyplot as plt

# Read data back from the file
x_new = []
y_new = []
with open('data2.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        x_new.append(int(data[0]))
        y_new.append(float(data[1]))

# Plot the new data
plt.plot(x_new, y_new, '.')
plt.xlabel('Prime')
plt.ylim(-10, 10)
plt.ylabel('Second moment')
plt.title('Second moment for x^3 -3(T^2 + 12)x - 2T(T^2 + 12)')
plt.show()
#plt.pause(10)  # Display the plot for 10 seconds
plt.ioff()

