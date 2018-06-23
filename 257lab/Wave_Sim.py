import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.signal import butter, filtfilt
from dataAnalysis import get_data
import random


"""
This is a simulation template that we can use if we need to,
it basically sets the perams in heating_rod_sim and modulates the power
in an if statement

"""


def mask(k, data1, data2):
    timeCopy = []
    tempCopy = []

    count = 0
    for x, y in zip(data1, data2):
        if (count + random.randint(1, 5)) % k == 0:
            timeCopy.append(x)
            tempCopy.append(y)
        count += 1

    return np.array(timeCopy), np.array(tempCopy)


def show(filename="goodData/Horizontal_heating_steady_state.csv"):

    filename = input("please enter the file name: ")
    params = [0] * 5
    vals = input("Enter k_c, power, epi, c, k and density as well as T_0, T_1 and rest of T each followed by a comma: ")

    params = vals.split(",")
    print(str(params))
    offset = float(input("Enter the offset in ms for the data to start: "))
    params = [float(x) for x in params]
    assert len(params) == 9

    data = get_data(filename)
    Temp = data[2]
    time1 = (data[4] - offset) / 1000
    time = np.array([x for x in time1[:, :] if x >= -offset])

    time = time.reshape(len(time), 1)
    Temp = Temp[len(time1) - len(time):, :]

    time, Temp = mask(100, time, Temp)

    plt.figure(figsize=(10, 10))

    # plt.scatter(time, 30 * POWER)
    plt.title(input("Enter the title: "), fontsize=15)
    plt.xlabel("Time [s]", fontsize=18)
    plt.ylabel("Temperature [$^oC$]", fontsize=18)
    plt.grid('on')

    fit = heating_rod_sim(k_c=params[0], power=params[1], epi=params[2], c=params[3], k=params[4], p=params[5], T_0=params[6], T_1=params[7], T=params[8])
    size = np.ones(time.shape) * 30

    # plt.scatter([0.3 - 0.013, 0.3 - 0.083, 0.3 - 0.153, 0.3 - 0.222, 0.3 - 0.2925] * np.ones(Temp.shape), Temp[-1], label="Data @ steady state")

    # plt.scatter(time, 40 * data[3], c="k", label="power")
    error = 0.5
    e1 = 15
    # plt.style.use('presentation')
    plt.style.use('ggplot')
    width = list(np.ones(len(fit[-1])) * 10)

    plt.scatter(time, Temp[:, 4:5], c="m", marker="o", label="1 cm", s=size)
    plt.errorbar(time, Temp[:, 4:5], yerr=error, c="m", errorevery=e1, fmt='none', capsize=5, capthick=3)
    plt.plot(fit[-1], fit[-2][:, 1:2], "m-", linewidth=3)

    plt.scatter(time, Temp[:, 3:4], c="r", marker="o", label="8 cm", s=size)
    plt.errorbar(time, Temp[:, 3:4], yerr=error, c="r", errorevery=1.2 * e1, fmt='none', capsize=5, capthick=3)
    plt.plot(fit[-1], fit[-2][:, 8:9], "r-", linewidth=3)

    plt.scatter(time, Temp[:, 2:3], c="c", marker="o", label="15 cm", s=size)
    plt.errorbar(time, Temp[:, 2:3], yerr=error, c="c", errorevery=1.3 * e1, fmt='none', capsize=5, capthick=3)
    plt.plot(fit[-1], fit[-2][:, 15:16], "c-", linewidth=3)

    plt.scatter(time, Temp[:, 1:2], c="g", marker="o", label="22 cm", s=size)
    plt.errorbar(time, Temp[:, 1:2], yerr=error, c="g", errorevery=1.4 * e1, fmt='none', capsize=5, capthick=3)
    plt.plot(fit[-1], fit[-2][:, 23:24], "g-", linewidth=3)

    plt.scatter(time, Temp[:, 0:1], c="b", marker="o", label="29 cm", s=size)
    plt.errorbar(time, Temp[:, 0:1], yerr=error, c="b", errorevery=1.5 * e1, fmt='none', capsize=5, capthick=3)
    plt.plot(fit[-1], fit[-2][:, 29:30], "b-", linewidth=3)

    plt.axis([-15, 147 * 2 * 3, 20, 45])
    plt.yticks(np.linspace(20, 45, 26))
    # for i in range(5):
    #    plt.scatter(time, Temp[:, i:i + 1], label="Sensor: {}".format(i + 1))

    plt.legend(loc=2, prop={'size': 15})

    plt.show()


def heating_rod_sim(k_c=0, power=0, epi=0, c=0, k=0, p=0, T_0=0, T_1=1, T=0, show=False):
    L = 0.3  # length of rod
    inital = T_0 + 273.15
    Dx = 0.01  # steps of x
    N = int(L / Dx)  # the number of steps
    Dt = 0.04  # steps in time

    time_s = []
    Temps = []

    # k = 200.0  # W / m / K
    # c = 900.0  # J / kg / K
    # p = 2600.0  # kg / m^3

    period = 147.0 * 2
    r = 0.02535  # m radius
    # power = 15.0
    Pin = power  # W  power in
    nu = 0.8  # 80% the efficiency of the power resistor to transfer into the rod
    T_amb = 22.0 + 273.15  # ambient temp
    # k_c = 5   # W/m^2/K convection constant
    # epi = 0.1  # emissivity
    sigma = 5.67 * 10 ** (-8)  # W/m^2/K (stefan-Boltzmann constant)

    C = k / (c * p)  # constant
    x = np.linspace(0.0, L, N)

    T = [inital] + [T_1 + 273.15] + [T + 273.15] * (N - 2)
    T = np.array(T)

    t = 0
    delay = 1000
    last_change = 0
    if show:
        plt.figure(figsize=(20, 20))
    while True:
        if t > delay:
            print("time: " + str(t))

        # this is to modulate the power

        for j in range(10000):
            for i in range(1, N - 1):

               # k_c = 1.32 * ((T[i] - T_amb) / (2 * r)) ** (1 / 4)

                T[i] = T[i] + C * Dt * (T[i - 1] - 2 * T[i] + T[i + 1]) / (Dx ** 2)  # heat transfer (conduction)

                T[i] = T[i] - 2 * Dt * k_c * (T[i] - T_amb) / (c * p * r)  # convection loss

                T[i] = T[i] - 2 * Dt * epi * sigma * (T[i] ** (4) - T_amb ** (4)) / (c * p * r)  # radiation loss

            T[0] = T[0] - C * Dt * (T[0] - T[1]) / (Dx ** 2)  # heat transfer (conduction)loss

            T[0] = T[0] + nu * Pin * Dt / (c * p * math.pi * r ** (2) * Dx)  # % power gain
            # k_c = 1.32 * ((T[0] - T_amb) / (2 * r)) ** (1 / 4)

            T[0] = T[0] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * k_c * (T[0] - T_amb)  # convection loss

            T[0] = T[0] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * epi * sigma * (T[0]**(4) - T_amb ** 4)  # radiation loss
            # k_c = 1.32 * ((T[-1] - T_amb) / (2 * r)) ** (1 / 4)

            T[-1] = T[-1] + C * Dt * (T[-2] - T[-1]) / (Dx**2)  # heat gain (conduction)
            T[-1] = T[-1] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * k_c * (T[-1] - T_amb)  # convection loss

            T[-1] = T[-1] - Dt * (2 / (c * r * p) + 1 / (c * p * Dx)) * epi * sigma * (T[-1]**(4) - T_amb ** 4)  # radiation loss

            if(int(t + 1) % (period // 2) == 0 and Pin == 0 and int(t + 1) != 0 and t > last_change):
                last_change = t + 1
                Pin = power
            elif(int(t + 1) % (period // 2) == 0 and Pin == power and int(t + 1) != 0 and t > last_change):
                last_change = t + 1
                Pin = 0

            t += Dt
            time_s.append(t)
            Temps.append(np.array(list(map(lambda x: (x - 273.15), T))))

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
        if t >= period * 3 and not show:
            return x, np.array(Temps).reshape(len(time_s), N), time_s


if __name__ == "__main__":
    show()
