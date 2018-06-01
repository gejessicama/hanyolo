import serial
import numpy as np
import matplotlib as mtb
import matplotlib.pyplot as plt
import pylab as pyl
import time

# initialize serial
port_name = "COM5"
baudrate = 9600
ser = serial.Serial(port_name, baudrate)

# creates file with dated name
timestr = time.strftime("%Y%m%d-%H%M")
datafile = open(timestr + ".csv", "w+")
datafile.write("A1,A2,A3,A4,A5,time\n")

X = [1, 2, 3, 4, 5]
# read data into an array


def read_data():
    data = ser.readline().decode()
    while data.isspace():  # if faulty reading (whitespace), keep trying
        data = ser.readline().decode()
    datafile.write(data)
    try: 
        return list(map(float, data.split(",")))
    except ValueError:
        return read_data()

plt.show()

while True:
    msmt = read_data()
    print(msmt)
    plt.title("Temperature of a vertical Rod as a function of distance and time", fontsize=15)
    plt.xlabel("Sensor", fontsize=12)
    plt.ylabel("Temperature ($^oC$)", fontsize=12)
    plt.grid('on')
    plt.scatter(X, msmt[:5])
    plt.axis([0,6,0,50])
    plt.pause(0.01)
    plt.clf()

'''
def addPoint(xs, ys, axis, colour="r", shape="o"):
    """
    animate the plot of the error

    @param xs a list of x points
    @param ys a list of y points
    @param axis i.e suplot
    """
    axis.plot(xs, ys, colour + shape)
    return True

        fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
        fig.set_size_inches(10, 10)
        ax1.set_title("RMS Error")
        ax1.set(ylabel="Error")
        ax2.set_title("Learning Rate")
        ax2.set(ylabel="Rate")
        plt.xlabel("Epoch number")
        plt.tight_layout()
        # plt.draw()
        animate.FuncAnimation(fig, addPoint)  # animate the function
'''
