import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mt
from matplotlib import cm
import csv
import mpl_toolkits.mplot3d.axes3d as p3
from matplotlib import animation


def plot(filename):
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
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        data = list(reader)
        data2 = []
        for line in data:
            l = [x for x in line if x != '']
            if len(l) == 6:
                data2.append(l)

        for i in range(1, len(data2)):
            data2[i] = list(map(float, data2[i]))

    with open(output, 'w') as file:
        writer = csv.writer(file)
        writer.writerows(data2)


def main(Animate=False):
    filename = input("please enter the file name: ")

    # fix(filename)

    data = plot(filename)
    Temp = data[:, :5].reshape(data[:, :5].size // 5, 5)
    time = data[:, 5:].reshape(data[:, 5:].size, 1)
    x = np.ones(Temp.shape) * [1, 2, 3, 4, 5]
    x = x.reshape(x.size // 5, 5)

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
            Z = np.array(Temp[:N, :])

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

            for i in range(5):
                ax.scatter(X[:, i:i + 1], Y[:, :], Z[:, i:i + 1], c=Z[:, i:i + 1].reshape(Z[:, i:i + 1].size,), cmap="plasma")

            #ax.plot_surface(X, Y, Z, cmap=cm.plasma)

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

        for i in range(5):
            ax.scatter(x[:, i:i + 1], time[:, :], Temp[:, i:i + 1], c=Temp[:, i:i + 1].reshape(Temp[:, i:i + 1].size,), cmap="plasma")
        #ax.plot_surface(x, time, Temp, cmap=cm.plasma)
        plt.show()

if __name__ == '__main__':
    main()
