import serial
import numpy as np
import matplotlib as mtb
import pylab as pyl

# initialize serial 
port_name = "COM4"
baudrate = 9600
ser = serial.Serial(port_name,baudrate)

# creates file with dated name
timestr = time.strftime("%Y%m%d-%H%M")
datafile = open(timestr + ".csv", "w+")
datafile.write("A0,A1,A2,A3,A4,time\n")

# read data into an array 
def read_data():
	data = ser.readline().decode()
	while data.isspace(): # if faulty reading (whitespace), keep trying
		data = ser.readline().decode()
	datafile.write(data)
	return list(map(int, data.split(",")))

msmt = read_data()
datafile.write(msmt)
X = [1,2,3,4,5]

plt.ion()
graph = plt.plot(X,msmt)[0]

while True:
    graph.set_ydata(msmt)
    plt.draw()
    plt.pause(0.01)