#import csv
#import itertools
import matplotlib.pyplot as plt

# Read data back from the file
x_new = []
y_new = []
sumofdata = 0
count = 0
title = input("What do you want the title to be? ")

with open('data.txt', 'r') as file:
    for line in file:
        data = line.strip().split(',')
        x_new.append(int(data[0]))
        y_new.append(float(data[1]))

# Plot the new data
plt.plot(x_new, y_new, '.')
plt.xlabel('Prime')
plt.ylabel('Running average of second moment at prime')
plt.axhline(y = 0)
plt.ylim(-.1, .1)
plt.title('Running average of second moment of ' + title)
plt.show()