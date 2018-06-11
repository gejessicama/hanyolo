import serial
import numpy as np
import matplotlib.pyplot as plt
import time
import csv
# initialize serial
port_name = "COM6"  # the port should check
baudrate = 9600
ser = serial.Serial(port_name, baudrate)
ser.flushInput()  # start a new reading

title = ["A1", "A2", "A3", "A4", "A5", "heat", "time"]

# creates file with dated name
timestr = time.strftime("%Y%m%d-%H%M")
datafile = open("data/" + timestr + ".csv", "w+")
writer = csv.writer(datafile, delimiter=",")
writer.writerow(["A1", "A2", "A3", "A4", "A5", "heat", "time"])
# datafile.write("A1,A2,A3,A4,A5,heat,time\n")

# x-axis
X = [1, 2, 3, 4, 5]

# read data into an array


def read_data():
    data = ser.readline().decode("utf-8")
    while data.isspace():  # if faulty reading (whitespace), keep trying
        data = ser.readline().decode("utf-8")
    # datafile.write(data)
    try:
        q = list(map(float, data.split(",")))
        if len(q) == len(title):
            writer.writerow(q)
            return q
        else:
            print("bad data: {}, len: {}".format(q, len(q)))
            return read_data()
    except ValueError:
        return read_data()

plt.show()

while True:
    # this loop takes about 68 ms
    try:
        msmt = read_data()
        print(msmt)
        plt.title("Temperature of a rod as a function of distance and time")
        plt.xlabel("Sensor #")
        plt.ylabel("Temperature ($^oC$)")
        plt.grid('on')
        try:
            plt.scatter(X, msmt[:5])
        except ValueError:
            pass
        plt.axis([0, 6, 0, 50])
        plt.pause(0.01)
        plt.clf()
    except KeyboardInterrupt:
        print("Good bye :)")
        break
