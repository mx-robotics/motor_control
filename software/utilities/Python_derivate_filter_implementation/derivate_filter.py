# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import matplotlib.pyplot as plt
import numpy as np
import random as rnd


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


class DiffFilter:
    def __init__(self):
        self.s = 0
        self.s_dot = 0
        self.delta_T = 1
        self.T_f = 5 # only thing to tune 

    def filter(self, measurement):
        s_temp = self.s + self.s_dot * self.delta_T # new value, old value + difference (s_dot * delta_T)
        s_dot_temp = self.s_dot + (
                -self.s / (self.T_f * self.T_f) - 2 * self.s_dot / self.T_f + measurement) * self.delta_T
        self.s = s_temp
        self.s_dot = s_dot_temp
        filtered_measurement = self.s / (self.T_f * self.T_f)
        return filtered_measurement


class DataCreator:
    def __init__(self, Filter):
        self.x = []
        self.y = []
        self.y_noise = []
        self.y_filtered = []
        self.filterObj = Filter
        self._createData()
        self._createNoiseyData()
        self._createFilteredData()

    def getPerfectData(self):
        return self.x, self.y

    def getNoiseyData(self):
        return self.x, self.y_noise

    def getFilteredData(self):
        return self.x, self.y_filtered

    def _createData(self):
        inc = 0
        for i in range(200):
            self.x.append(i)
            self.y.append(inc)
            inc += 10

    def _createNoiseyData(self):
        for i in range(len(self.x)):
            noise = rnd.randint(-100, 100)
            if i is 72:
                noise = 1000
            if i is 112:
                noise = 800
            if i is 32:
                noise = 800
            self.y_noise.append(self.y[i] + noise)

    def _createFilteredData(self):
        for i in range(len(self.x)):
            self.y_filtered.append(self.filterObj.filter(self.y_noise[i]))


# Press the green button in the gutter to run the script.


if __name__ == '__main__':
    print_hi('PyCharm')
    DiffFilterObj = DiffFilter()
    DataCreatorObj = DataCreator(DiffFilterObj)

    # x = np.linspace(0, 2 * np.pi, 400)
    # y = np.sin(x ** 2)
    a, b = DataCreatorObj.getPerfectData()
    fig, (ax1, ax2, ax3) = plt.subplots(3)
    ax1.plot(a, b)

    fig.suptitle('Vertically stacked subplots')
    a, b = DataCreatorObj.getNoiseyData()

    ax2.plot(a, b)
    l, k = DataCreatorObj.getFilteredData()
    ax3.plot(l, k)

    plt.show()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
