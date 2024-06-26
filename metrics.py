# Dylan Hughes and Liam Kelly
# Carleton University
# COMP4900 E
# April 2024

import time
import numpy as np
import matplotlib.pyplot as plt

import main


def graph():
    nRuns = 10
    nAlgs = 9
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
        main.batch_process('./test', 'shake_192f')
        process_times[1][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'shake_192f')
        verify_times[1][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'shake_256f')
        process_times[2][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'shake_256f')
        verify_times[2][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'haraka_128f')
        process_times[3][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'haraka_128f')
        verify_times[3][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'haraka_192f')
        process_times[4][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'haraka_192f')
        verify_times[4][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'haraka_256f')
        process_times[5][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'haraka_256f')
        verify_times[5][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'sha2_128f')
        process_times[6][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'sha2_128f')
        verify_times[6][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'sha2_192f')
        process_times[7][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'sha2_192f')
        verify_times[7][i] = time.time() - startTime

        startTime = time.time()
        main.batch_process('./test', 'sha2_256f')
        process_times[8][i] = time.time() - startTime

        startTime = time.time()
        main.batch_verify('./test', 'sha2_256f')
        verify_times[8][i] = time.time() - startTime

    for i in range(nAlgs):
        processAvgs[i] = np.average(process_times[i])
        verifyAvgs[i] = np.average(verify_times[i])


    labels= ['Shake128', 'Shake192', 'Shake256', 'Haraka128', 'Haraka192', 'Haraka256', 'Sha2-128', 'Sha2-192', 'Sha2-256']

    plt.figure(figsize=(10, 5))
    plt.bar(labels, processAvgs, color='red', width=0.7)
    plt.ylabel("Time taken (Sec)")
    plt.title("Time vs Signing with SPHINCS+")
    plt.savefig('sign_plot.png')
    plt.close()

    plt.figure(figsize=(11, 5))
    plt.bar(labels, verifyAvgs, color='blue', width=0.7)
    plt.ylabel("Time taken (Sec)")
    plt.title("Time vs Verifying with SPHINCS+")
    plt.savefig('verify_plot.png')
    plt.close()


if __name__ == '__main__':
    graph()