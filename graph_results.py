import csv
import math
from operator import itemgetter
from itertools import groupby
import matplotlib.pyplot as plt
plt.rcParams["figure.figsize"] = (8.2, 10)

from perf_test import operations, thread_numbers, input_sizes

VERSION = "v1"

with open(f"./{VERSION}-stats/{VERSION}-results.csv", "r", newline="") as csv_fileobj:
    reader = csv.reader(csv_fileobj)
    next(reader)
    rows = list(reader)

def plt_op_hist(x, y, op, size, index=1):
    trials = [r for r in rows if r[0] == op and r[1] == size]
    n_thread_to_trials = {k: list(g) for k, g in groupby(trials, key=itemgetter(2))
                          if k in thread_numbers}
    duration_lists = [
        [float(trial[-1]) * 1000 for trial in trials]
        for _, trials in sorted(n_thread_to_trials.items(), key=itemgetter(0))
    ]
    ax = plt.subplot(x, y, index)
    labels = [f"{n} Thread(s)" for n in thread_numbers]
    ax.hist(duration_lists, label=labels)
    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Frequency")
    ax.set_title(f"$\it{{{op}}}$ (int{size}) Threading Performance")
    ax.legend(prop={"size": 10})

if __name__ == "__main__":
    dim = math.ceil(len(input_sizes) / 2)
    for operation in operations:
        plt.clf()
        plot_index = 1
        for input_size in input_sizes:
            plt_op_hist(dim, dim, operation, input_size, plot_index)
            plot_index += 1
        plt.savefig(f"./{VERSION}-stats/{operation}.png", quality=100, papertype="a4", orientation="landscape")