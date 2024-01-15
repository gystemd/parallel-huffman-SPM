
import matplotlib.pyplot as plt
import numpy as np

thread_folders = "measurements/threads/"
threads = []
times = []

# read the csv file
thread_num = 2
with open("measurements/threads.csv", "r") as f:
    for line in f:
        threads.append(thread_num)
        times.append(sum([float(x) for x in line.split(",")]))
        print (sum([float(x) for x in line.split(",")]))
        thread_num *= 2


x_ticks = np.linspace(min(threads), max(threads), len(threads))
# plot the data
plt.plot(threads, times, marker='o')  # Plot points on the x-axis
plt.xlabel('Number of threads')
plt.xticks(x_ticks)
plt.ylabel('Time (us)')
# plt.yscale('log')
plt.savefig('figures/threads-IO.png')
plt.clf()

times = []
threads = []
thread_num = 2

with open("measurements/threads.csv", "r") as f:
    for line in f:
        threads.append(thread_num)
        times.append(sum([float(x) for x in line.split(",")[1:-1]]))
        print (sum([float(x) for x in line.split(",")]))
        thread_num *= 2


plt.plot(threads, times)
plt.xlabel('Number of threads')
plt.ylabel('Time (us)')
plt.yscale('log')
plt.savefig('figures/threads-nIO.png')
plt.clf()