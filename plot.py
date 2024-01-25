
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import numpy as np
import pandas as pd
sequential_data = pd.read_csv('measurements/seq.csv', sep=',', header=None)
total_time_sequential = sequential_data.iloc[0, 2]
threads_data = pd.read_csv('measurements/threads.csv', sep=',', header=None)
ff_data = pd.read_csv('measurements/ff.csv', sep=',', header=None)

plt.plot(threads_data.iloc[:, 0], threads_data.iloc[:, 2])
plt.gca().yaxis.set_major_formatter(mtick.FormatStrFormatter('%.0f'))
plt.xlabel('number of workers')
plt.ylabel('total time')
plt.savefig('figures/threads.png')
plt.clf()

plt.plot(ff_data.iloc[:, 0], ff_data.iloc[:, 2])
plt.gca().yaxis.set_major_formatter(mtick.FormatStrFormatter('%.0f'))
plt.xlabel('number of workers')
plt.ylabel('total time')
plt.savefig('figures/ff.png')
plt.clf()

speedup_threads = total_time_sequential / threads_data.iloc[:, 2]
plt.plot(threads_data.iloc[:, 0], speedup_threads)
plt.xlabel('threads')
plt.ylabel('speedup')
plt.savefig('figures/speedup_threads.png')

speedup_ff = total_time_sequential / ff_data.iloc[:, 2]
plt.plot(ff_data.iloc[:, 0], speedup_ff)
plt.xlabel('threads')
plt.ylabel('speedup')
plt.savefig('figures/speedup_ff.png')
