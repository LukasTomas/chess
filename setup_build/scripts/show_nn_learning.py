#!/usr/bin/env python3.9

import time
import matplotlib.pyplot as plt
import sys

MSES_FILEPATH = "../values.mse"

def show_graph(mse_values):
    plt.clf()
    plt.plot(list(range(len(mse_values))), mse_values, marker='o')

    plt.xlabel('Satisfied After')
    plt.ylabel('Probability')

    # set xticks 1 - 53 steps 2
    #axes.set_xticks(np.arange(1, 53, 2))
    plt.pause(1)

def follow(thefile):
    thefile.seek(0,2)
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line

def main():
    only_last_line = False
    if len(sys.argv) > 1 and sys.argv[1] == 'last':
        only_last_line = True

    mse_file = open(MSES_FILEPATH, 'r')
    mse_values = []

    iterator = follow(mse_file) if only_last_line else mse_file
    for line in iterator:
        line = line[:-1]    # remove \n
        mse: float = float(line)
        print('mse value: ' + str(mse))

        mse_values.append(mse)

        show_graph(mse_values)

    return 0

if __name__ == "__main__":
    main()
