# Dylan Hughes and Liam Kelly
# Carleton University
# COMP4900 E
# April 2024

import time
import os
import numpy as np
import matplotlib.pyplot as plt

import main


def graph():
    nRuns = 10
    nAlgs = 2
    times = np.zeros([nAlgs, nRuns])
    timeAvgs = np.zeros([nAlgs])

    for i in range(nRuns):
        startTime = time.time()
        main.batch_process('./test')
        times[0][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'shake_256f')
        times[1][i] = time.time() - startTime

    for i in range(nAlgs):
        timeAvgs[i] = np.average(times[i])

    labels= ['Shake128', 'Shake256']

    plt.figure()
    plt.bar(labels, timeAvgs, color='red')

    plt.ylabel("Time taken (Sec)")

    plt.title("Time vs Different SPHINCS+ hashes")
    plt.savefig('pqc_plot.png')


if __name__ == '__main__':
    graph()