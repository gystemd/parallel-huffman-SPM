
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import numpy as np
import pandas as pd
sequential_data = pd.read_csv('measurements/seq.csv', sep=',', header=None)
total_time_sequential = sequential_data.iloc[0, 2]
threads_data = pd.read_csv('measurements/threads.csv', sep=',', header=None)
# Assuming that the first column is named 'column1' and the second column is 'column2'
plt.plot(threads_data.iloc[:, 0], threads_data.iloc[:, 2])
plt.gca().yaxis.set_major_formatter(mtick.FormatStrFormatter('%.0f'))

plt.xlabel('threads')
plt.ylabel('total time')
# save the plot as a file
plt.savefig('threads.png')

plt.clf()

speedup = total_time_sequential / threads_data.iloc[:, 2]
plt.plot(threads_data.iloc[:, 0], speedup)
plt.xlabel('threads')
plt.ylabel('speedup')
# save the plot as a file
plt.savefig('speedup.png')
