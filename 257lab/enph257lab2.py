import serial
import numpy as np
import matplotlib as mtb
import matplotlib.pyplot as plt
import pylab as pyl
import time

plt.ion()
plt.figure(figsize=(20, 20))
plt.title("Temperature of a vertical Rod as a function of distance and time", fontsize=20)
plt.xlabel("Sensor", fontsize=18)
plt.ylabel("Temperature ($^oC$)", fontsize=18)
plt.grid('on')

# initialize serial
port_name = "COM5"
baudrate = 9600
ser = serial.Serial(port_name, baudrate)

# creates file with dated name
timestr = time.strftime("%Y%m%d-%H%M")
datafile = open(timestr + ".csv", "w+")
datafile.write("A1,A2,A3,A4,A5,time\n")

# read data into an array


def read_data():
    data = ser.readline().decode()
    while data.isspace():  # if faulty reading (whitespace), keep trying
        data = ser.readline().decode()
    datafile.write(data)
    return list(map(int, data.split(",")))


msmt = [0] * 5
temp = msmt[:6]
X = [1, 2, 3, 4, 5]
graph = plt.plot(X, temp)[0]


while True:
    msmt = read_data()
    temp = msmt[:6]
    graph.set_ydata(temp)
    plt.draw()
    plt.pause(0.01)
