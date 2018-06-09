import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.signal import butter, filtfilt
from dataAnalysis import get_data


def show(filename="goodData/Horizontal_heating_steady_state.csv"):
    #filename = input("please enter the file name: ")
    data = get_data(filename)
    Temp = data[2]
    time = data[4] / 1000

    plt.figure(figsize=(10, 10))

    #plt.scatter(time, 30 * POWER)
    plt.title("Steady state Temperature versus position on an Aluminum rod", fontsize=15)
    plt.xlabel("position (m)", fontsize=18)
    plt.ylabel("Temperature $^oC$", fontsize=18)
    plt.grid('on')

    fit = heating_rod_sim()

    plt.scatter([0.3 - 0.013, 0.3 - 0.083, 0.3 - 0.153, 0.3 - 0.222, 0.3 - 0.2925], Temp[-1], label="Data @ steady state")
    plt.plot(fit[0], list(map(lambda x: (x - 273.15), fit[1])), "r--", label="Fit for the data")

    #plt.axis([0, time[-1] + 10, 0, 70])
    # for i in range(5):
    #    plt.scatter(time, Temp[:, i:i + 1], label="Sensor: {}".format(i + 1))

    plt.legend(loc=0, prop={'size': 15})

    plt.show()


def heating_rod_sim(show=False):
    L = 0.3  # length of rod
    inital = 21.9 + 273.15
    Dx = 0.01  # steps of x
    N = int(L / Dx)  # the number of steps
    Dt = 0.04  # steps in time

    k = 80.0  # W / m / K
    c = 900.0  # J / kg / K
    p = 2650.0  # kg / m^3

    #period = 60
    r = 0.02535  # m radius
    Pin = 15.0  # W  power in
    T_amb = 23.0 + 273.15  # ambient temp
    k_c = 6.8   # W/m^2/K convection constant
    epi = 0.10  # emissivity
    sigma = 5.67 * 10 ** (-8)  # W/m^2/K (stefan-Boltzmann constant)

    C = k / (c * p)  # constant
    x = np.linspace(0.0, L, N)

    T = [inital] + [21.9 + 273.15] * (N - 1)
    T = np.array(T)

    t = 0
    delay = 4999
    if show:
        plt.figure(figsize=(20, 20))
    while True:
        if t > delay:
            print("time: " + str(t))

        # this is to modulate the power
        """
        if((t + 1) % period // 2 == 0 and Pin == 0):
            Pin = 12
        elif((t + 1) % period // 2 == 0 and Pin == 12):
            Pin = 0
        """

        for j in range(10000):
            for i in range(1, N - 1):

               # k_c = 1.32 * ((T[i] - T_amb) / (2 * r)) ** (1 / 4)

                T[i] = T[i] + C * Dt * (T[i - 1] - 2 * T[i] + T[i + 1]) / (Dx ** 2)  # heat transfer (conduction)

                T[i] = T[i] - 2 * Dt * k_c * (T[i] - T_amb) / (c * p * r)  # convection loss

                T[i] = T[i] - 2 * Dt * epi * sigma * (T[i] ** (4) - T_amb ** (4)) / (c * p * r)  # radiation loss

            T[0] = T[0] - C * Dt * (T[0] - T[1]) / (Dx ** 2)  # heat transfer (conduction)loss

            T[0] = T[0] + Pin * Dt / (c * p * math.pi * r ** (2) * Dx)  # % power gain
            #k_c = 1.32 * ((T[0] - T_amb) / (2 * r)) ** (1 / 4)

            T[0] = T[0] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * k_c * (T[0] - T_amb)  # convection loss

            T[0] = T[0] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * epi * sigma * (T[0]**(4) - T_amb ** 4)  # radiation loss
            #k_c = 1.32 * ((T[-1] - T_amb) / (2 * r)) ** (1 / 4)

            T[-1] = T[-1] + C * Dt * (T[-2] - T[-1]) / (Dx**2)  # heat gain (conduction)
            T[-1] = T[-1] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * k_c * (T[-1] - T_amb)  # convection loss

            T[-1] = T[-1] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * epi * sigma * (T[-1]**(4) - T_amb ** 4)  # radiation loss

            t += Dt

        if t >= delay and show:

            plt.title(
                "Steady state Temperature versus position on an Aluminum rod",
                fontsize=30)
            plt.xlabel("Distance from start (m)", fontsize=18)
            plt.ylabel("Temperature (deg C)", fontsize=18)
            plt.grid('on')

            plt.axis([0, L, 0, 100])
            plt.plot(
                x, T - 273.15 * np.ones(T.shape), "b-", label="Left End: {}, Right End: {} , time: {} s".format(T[0] - 273.15, T[-1] - 273.15, t))
            plt.legend(loc=0, prop={'size': 15})
            plt.pause(0.01)
            plt.clf()
        if t >= delay and not show:
            return x, T


if __name__ == "__main__":
    show()
