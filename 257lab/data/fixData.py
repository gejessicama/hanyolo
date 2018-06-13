import numpy as np
import csv
import re


Colums = 7


def fix(filename, output='fixed.csv'):
    """
    This method tries to fix the csv data by getting rid of
    the lines that contain more than 7 numbers.
    """
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        data = list(reader)
        data2 = []
        for line in data:
            l = [x.strip() for x in line if x != '' and ".." not in x and ',' not in x and not bool(re.search(r'\.\d+\.', x))]
            if len(l) == Colums:
                data2.append(l)
            else:
                pass
                # print("{}".format(l))

            data3 = []

        for i in range(1, len(data2)):

            data2[i] = list(map(float, data2[i]))
            #nums = [x for x in data2[i] if x >= 10]
            if(data2[i][-2] > 0):  # if the power column is not zero
                data3.append(data2[i])

            # if len(nums) == Colums:
            #     data3.append(nums)

    with open(output, 'w') as file:
        writer = csv.writer(file)
        writer.writerows(data3)

if __name__ == "__main__":

    fix(input("Enter the filename: "))
