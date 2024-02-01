
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import numpy as np
import pandas as pd

seq_data = pd.read_csv('measurements/seq-nojemalloc.csv', sep=',', header=None).groupby(0).mean().rename_axis(None)

tot_time_seq = seq_data.iloc[0, 1]
seq_data_jemalloc = pd.read_csv('measurements/seq-jemalloc.csv', sep=',', header=None).groupby(0).mean()
tot_time_seq_jemalloc = seq_data_jemalloc.iloc[0, 1]

threads_data = pd.read_csv('measurements/threads-nojemalloc.csv', sep=',', header=None).groupby(0).mean()
threads_data_jemalloc = pd.read_csv('measurements/threads-jemalloc.csv', sep=',', header=None).groupby(0).mean()

ff_data = pd.read_csv('measurements/ff-nojemalloc.csv', sep=',', header=None).groupby(0).mean()
ff_data_jemalloc = pd.read_csv('measurements/ff-jemalloc.csv', sep=',', header=None).groupby(0).mean()

plt.gca().yaxis.set_major_formatter(mtick.FormatStrFormatter('%.0f'))
plt.grid(True)

plt.xlabel('number of workers')
plt.ylabel('total time')
plt.plot(threads_data.index, threads_data.iloc[:, 1], label='threads', color='0.0', marker='o')
plt.plot(threads_data_jemalloc.index, threads_data_jemalloc.iloc[:, 1], label='threads jemalloc', color='0.25', marker='s')
plt.plot(ff_data.index,  ff_data.iloc[:, 1], label='ff', color='0.5', marker='^')
plt.plot(ff_data_jemalloc.index, ff_data_jemalloc.iloc[:, 1], label='ff jemalloc', color='0.75', marker='*')
plt.legend()
plt.savefig('figures/total-time.png')
plt.clf()

plt.grid(True)
plt.xlabel('threads')
plt.ylabel('speedup')
speedup_threads = tot_time_seq / threads_data.iloc[:, 1]
plt.plot(threads_data.index, speedup_threads, label='threads', color='0.0', marker='o')
speedup_threads_jemalloc = tot_time_seq_jemalloc / threads_data_jemalloc.iloc[:, 1]
plt.plot(threads_data_jemalloc.index, speedup_threads_jemalloc, label='threads jemalloc', color='0.25', marker='s')
speedup_ff = tot_time_seq / ff_data.iloc[:, 1]
plt.plot(ff_data.index, speedup_ff, label='ff', color='0.5', marker='^')
speedup_ff_jemalloc = tot_time_seq_jemalloc / ff_data_jemalloc.iloc[:, 1]
plt.plot(ff_data_jemalloc.index, speedup_ff_jemalloc, label='ff jemalloc', color='0.75', marker='*')
plt.legend()
plt.savefig('figures/speedup.png')
plt.clf()

plt.grid(True)
plt.xlabel('threads')
plt.ylabel('efficiency')
efficiency_threads = (tot_time_seq / threads_data.index) / threads_data.iloc[:, 1]
plt.plot(threads_data.index, efficiency_threads, label='threads', color='0.0', marker='o')
efficiency_threads_jemalloc = (tot_time_seq_jemalloc / threads_data_jemalloc.index) / threads_data_jemalloc.iloc[:, 1]
plt.plot(threads_data_jemalloc.index, efficiency_threads_jemalloc, label='threads jemalloc', color='0.25', marker='s')
efficiency_ff = (tot_time_seq / ff_data.index) / ff_data.iloc[:, 1]
plt.plot(ff_data.index, efficiency_ff, label='ff', color='0.5', marker='^')
efficiency_ff_jemalloc = (tot_time_seq_jemalloc / ff_data_jemalloc.index) / ff_data_jemalloc.iloc[:, 1]
plt.legend()
plt.plot(ff_data_jemalloc.index, efficiency_ff_jemalloc, label='ff jemalloc', color='0.75', marker='*')
plt.savefig('figures/efficiency.png')
