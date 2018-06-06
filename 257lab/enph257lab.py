import serial
import numpy as np
import matplotlib.pyplot as plt
import time

# initialize serial
port_name = "COM5"
baudrate = 9600
ser = serial.Serial(port_name, baudrate)

# creates file with dated name
timestr = time.strftime("%Y%m%d-%H%M")
datafile = open("data/"+ timestr + ".csv", "w+")
datafile.write("A1,A2,A3,A4,A5,heat,time\n")

# x-axis
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
    plt.title("Temperature of a vertical rod as a function of distance and time")
    plt.xlabel("Sensor #")
    plt.ylabel("Temperature ($^oC$)")
    plt.grid('on')
    try:
        plt.scatter(X, msmt[:5])
    except ValueError:
        pass
    plt.axis([0,6,0,50])
    plt.pause(0.01)
    plt.clf()