
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import numpy as np
import pandas as pd

seq_data = pd.read_csv('measurements/seq-nojemalloc.csv', sep=',', header=None)
tot_time_seq = seq_data.iloc[0, 2]
seq_data_jemalloc = pd.read_csv('measurements/seq-jemalloc.csv', sep=',', header=None)
tot_time_seq_jemalloc = seq_data_jemalloc.iloc[0, 2]

threads_data = pd.read_csv('measurements/threads-nojemalloc.csv', sep=',', header=None)
threads_data_jemalloc = pd.read_csv('measurements/threads-jemalloc.csv', sep=',', header=None)

ff_data = pd.read_csv('measurements/ff-nojemalloc.csv', sep=',', header=None)
ff_data_jemalloc = pd.read_csv('measurements/ff-jemalloc.csv', sep=',', header=None)

plt.gca().yaxis.set_major_formatter(mtick.FormatStrFormatter('%.0f'))
plt.grid(True)

plt.xlabel('number of workers')
plt.ylabel('total time')
plt.plot(threads_data.iloc[:, 0], threads_data.iloc[:, 2], label='threads')
plt.plot(threads_data_jemalloc.iloc[:, 0], threads_data_jemalloc.iloc[:, 2], label='threads jemalloc')
plt.plot(ff_data.iloc[:, 0], ff_data.iloc[:, 2], label='ff')
plt.plot(ff_data_jemalloc.iloc[:, 0], ff_data_jemalloc.iloc[:, 2], label='ff jemalloc')
plt.legend()
plt.savefig('figures/total-time.png')
plt.clf()

plt.xlabel('threads')
plt.ylabel('speedup')
speedup_threads = tot_time_seq / threads_data.iloc[:, 2]
plt.plot(threads_data.iloc[:, 0], speedup_threads, label='threads')
speedup_threads_jemalloc = tot_time_seq_jemalloc / threads_data_jemalloc.iloc[:, 2]
plt.plot(threads_data_jemalloc.iloc[:, 0], speedup_threads_jemalloc, label='threads jemalloc')
speedup_ff = tot_time_seq / ff_data.iloc[:, 2]
plt.plot(ff_data.iloc[:, 0], speedup_ff, label='ff')
speedup_ff_jemalloc = tot_time_seq_jemalloc / ff_data_jemalloc.iloc[:, 2]
plt.plot(ff_data_jemalloc.iloc[:, 0], speedup_ff_jemalloc, label='ff jemalloc')
plt.legend()
plt.savefig('figures/speedup.png')
