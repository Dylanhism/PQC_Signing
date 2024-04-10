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
    process_times = np.zeros([nAlgs, nRuns])
    verify_times = np.zeros([nAlgs, nRuns])
    processAvgs = np.zeros([nAlgs])
    verifyAvgs = np.zeros([nAlgs])

    for i in range(nRuns):
        startTime = time.time()
        main.batch_process('./test', 'shake_128f')
        process_times[0][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'shake_128f')
        verify_times[0][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'shake_256f')
        process_times[1][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'shake_256f')
        verify_times[1][i] = time.time() - startTime

    for i in range(nAlgs):
        processAvgs[i] = np.average(process_times[i])
        verifyAvgs[i] = np.average(verifyAvgs[i])


    labels= ['Shake128', 'Shake256']

    plt.figure()
    plt.bar(labels, processAvgs, color='red')
    plt.ylabel("Time taken (Sec)")
    plt.title("Time vs Signing with SPHINCS+")
    plt.savefig('sign_plot.png')
    plt.close()

    plt.figure()
    plt.bar(labels, verifyAvgs, color='blue')
    plt.ylabel("Time taken (Sec)")
    plt.title("Time vs Verifying with SPHINCS+")
    plt.savefig('verify_plot.png')
    plt.close()


if __name__ == '__main__':
    graph()