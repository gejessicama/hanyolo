"""
This is a script to plot the data obtained from the Arduino on a 3d plot.
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mt
from matplotlib import cm
import csv
import mpl_toolkits.mplot3d.axes3d as p3
from matplotlib import animation
import re
from scipy.signal import butter, filtfilt


def plot(filename):
    """
    This method reads the csv file and turns it into
    an array to be used for plotting in main

    """
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        data = list(reader)

    for i in range(1, len(data)):
        try:
            data[i] = np.array(list(map(float, data[i])))
        except Exception as e:
            print(data[i])
            input()

    data2 = []
    for l in data[1:]:
        if len(l) != 0:
            data2.append(np.array(l))

    return np.array(data2)


def fix(filename, output='fixed.csv'):
    """
    This method tries to fix the csv data by getting rid of
    the lines that contain more than 6 numbers.
    This does not look at the values of the data to determine if the
    data is correct; Need to write a method for that in the future.
    """
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        data = list(reader)
        print("size of original:{}".format(len(data)))
        data2 = []
        for line in data:
            l = [x.strip() for x in line if x != '' and ".." not in x and ',' not in x and not bool(re.search(r'\.\d+\.', x))]
            if len(l) == 7:
                data2.append(l)
            else:
                pass
                # print("{}".format(l))

        for i in range(1, len(data2)):
            data2[i] = list(map(float, data2[i]))

    with open(output, 'w') as file:
        writer = csv.writer(file)
        writer.writerows(data2)
    print("size of fixed{}".format(len(data2)))


def main(Animate=True, smooth=False, scatter=False, fixData=False):
    """
    This is the main method that does the plotting.

    """
    filename = input("please enter the file name: ")
<<<<<<< HEAD
    if fixData:
        fix(filename)  # the output
=======

    fix(filename)
>>>>>>> 9a16d881fb3a2bfb8c0313eefafcf01cbab8a6d0

    data = plot(filename)
    Temp = data[:, :5].reshape(data[:, :5].size // 5, 5)
    time = data[:, 6:].reshape(data[:, 6:].size, 1)
    POWER = data[:, 5:6].reshape(data[:, 5:6].size, 1)
    x = np.ones(Temp.shape) * [1, 2, 3, 4, 5]
    x = x.reshape(x.size // 5, 5)

    if smooth:
        # smooth out the data
        def butter_lowpass(cutoff, fs, order=5):
            nyq = 0.5 * fs
            normal_cutoff = cutoff / nyq
            b, a = butter(order, normal_cutoff, btype='low', analog=True)
            return b, a

        def butter_lowpass_filtfilt(data, cutoff, fs, order=5):
            b, a = butter_lowpass(cutoff, fs, order=order)
            print(data.shape)
            y = filtfilt(b, a, np.ravel(data))
            return y

        cutoff = 1500
        fs = 50000
        Temp_smooth = [butter_lowpass_filtfilt(x, cutoff, fs) for x in Temp.transpose()]
        Temp_smooth = np.array(Temp_smooth)
        Temp_smooth = Temp_smooth.transpose()

    else:
        Temp_smooth = Temp

    fig = plt.figure(figsize=(10, 10))
    ax = p3.Axes3D(fig)
    ax.view_init(28, 159)

    if Animate == True:
        X = np.zeros((0, 0))
        Y = np.zeros((0, 0))
        Z = np.zeros((0, 0))

        line = ax.plot_wireframe(X, Y, Z, cmap=cm.hot)

        def init():
            return line.get_children()  # need this to make it work

        def update(N):
            X = np.array(x[:N, :])
            Y = np.array(time[:N, :])
            Z = np.array(Temp_smooth[:N, :])

            ax.clear()  # clear prev
            # settings
            ax.set_xlim3d(0, 6)
            ax.set_ylim3d(0, time[-1] + 10000)
            ax.set_zlim3d(0, 70)
            ax.set_title("Temperature of a Rod as a function of time and distance", fontsize=25)
            ax.set_xlabel('Sensor location', fontsize=18)
            ax.set_ylabel('Time (ms)', fontsize=18)
            ax.set_zlabel('Temp $^oC$', fontsize=18)

            # draw new
            if scatter:
                for i in range(5):
                    ax.scatter(X[:, i:i + 1], Y[:, :], Z[:, i:i + 1], c=Z[:, i:i + 1].reshape(Z[:, i:i + 1].size,), cmap="plasma")

            else:
                ax.plot_surface(X, Y, Z, cmap=cm.plasma)

            return line.get_children()  # need this to make it work

        ani = animation.FuncAnimation(fig, update, range(1, Temp.shape[0], 1000), interval=100, blit=True)
        plt.show()

    else:
        ax.set_xlim3d(0, 6)
        ax.set_ylim3d(0, time[-1] + 10000)
        ax.set_zlim3d(0, 70)
        ax.set_title("Temperature of a Rod as a function of time and distance", fontsize=25)
        ax.set_xlabel('Sensor location', fontsize=18)
        ax.set_ylabel('Time (ms)', fontsize=18)
        ax.set_zlabel('Temp $^oC$', fontsize=18)

        #size = [20 for i in range(POWER.size)]
        #plt.scatter(time, 30 * POWER, s=size)

        if scatter:
            for i in range(5):
                ax.scatter(x[:, i:i + 1], time[:, :], Temp_smooth[:, i:i + 1], c=Temp[:, i:i + 1].reshape(Temp[:, i:i + 1].size,), cmap="plasma")
                # plt.scatter(time, Temp[:, i:i + 1], label=i)

        else:
            ax.plot_surface(x, time, Temp, cmap=cm.plasma)
        # plt.axis([time[5000], time[5000] + 128 * 1000, 0, 40])
        # plt.legend("best")
        # plt.grid()
        plt.show()

if __name__ == '__main__':
    main()
