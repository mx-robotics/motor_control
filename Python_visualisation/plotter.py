import tkinter
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from random import randrange
import serial
import serial.tools.list_ports
import struct


class MyClass:
    def __init__(self,):
        self.fig = plt.figure()
        # plt.title('Interesting Graph\nCheck it out')
        self.ax1 = None
        self.ax2 = None
        # for ax in self.ax1:
        #   print(ax)
        self.axii_names = []
        self.labels = []
        self.bytes_to_receive = 0
        self.configure_for_uin16_t() # configure_for_floats()
        self.num_of_axii = len(self.axii_names)
        self.arrays = {}
        self.create_the_arrays()
        self.ctr = 0
        self.plot_key = False
        self.portName = ""
        self.plot_key_counter = 0
        # self. baudrate = 115200

        self.ser = None
        self.establish_serial_port_com("ttyACM0")
        self.ani_func = FuncAnimation(self.fig, self.animate, interval=20)

        plt.legend()
        plt.show()

    def configure_for_floats(self):
        self.ax1 = self.fig.add_subplot(2, 1, 1)
        self.ax2 = self.fig.add_subplot(2, 1, 2)
        self.axii_names = ['x', 'y', 'z', 'e', 'c']
        self.labels = ["Measured", "Target", "Error", "Command"]
        self.bytes_to_receive = 160
    def configure_for_uin16_t(self):
        self.ax1 = self.fig.add_subplot(1, 1, 1)
        self.axii_names = ['x', 'y', 'z', 't']
        self.labels = ["Scaled", "Calculated", "DutyCycleV"]
        self.bytes_to_receive = 6

    def establish_serial_port_com(self, port_name_kw):
        self.bytes_to_receive = 160
        self.portName = ""
        self.get_portname_with_keyword(port_name_kw)
        self.ser = serial.Serial(self.portName)

    def get_portname_with_keyword(self, kw):
        ports = serial.tools.list_ports.comports(include_links=False)
        print("List of Serial Ports: ")
        for port in ports:
            print(port)
            if kw in port.device:
                self.portName = port.device

    def create_the_arrays(self):
        for i in range(self.num_of_axii):
            axis_name = self.axii_names[i]
            self.arrays[axis_name] = []

    def get_println_data(self):
        if self.ser.in_waiting:
            for i in self.axii_names:
                if i != 'x':
                    data = self.ser.readline()[0:-2]
                    data2 = int(data)
                    #int.from_bytes(data, byteorder='big')
                    #print(data)
                    self.arrays[i].append(data2)
                if i == 'x':
                    self.arrays[i].append(self.ctr)
                    self.ctr += 1
            if len(self.arrays['x']) > 100:
                self.plot_key = True

    def get_serial_uint16_data_to_plot(self):
        if self.ser.in_waiting:
            num_bytes_received = self.ser.in_waiting
            b1 = self.ser.read(self.ser.in_waiting)
            i= 0
            while i < (num_bytes_received / 2):
                try:
                    for j in range(1,len(self.axii_names)):
                        [k] = struct.unpack('H', b1[i * 2:(i + 1) * 2])
                        self.arrays[self.axii_names[j]].append(k)
                        i += 1
                        print(k)
                    self.arrays['x'].append(
                        self.ctr)  # this is the x axis representing time, determined by micro controllers send freq
                    self.ctr += 1  # milisecs

                except struct.error:
                    print("i = ", i)
                    print("exception thrown")
                    pass
            self.plot_key = True

    def get_serial_float_data_to_populate_arrays(self):
        if self.ser.in_waiting:  # Check for data not for an open port
            num_bytes_received = self.ser.in_waiting
            b1 = self.ser.read(self.ser.in_waiting)
            i = 0
            while i < (num_bytes_received / 4):
                try:
                    for j in range(1,len(self.axii_names)):
                        [k] = struct.unpack('f', b1[i * 4:(i + 1) * 4])
                        self.arrays[self.axii_names[j]].append(k)
                        i += 1

                    self.arrays['x'].append(self.ctr) # this is the x axis representing time, determined by micro controllers send freq
                    self.ctr += 0.1 # milisecs
                except struct.error:
                    print("i = ", i)
                    print("exception thrown")
                    pass
            self.plot_key = True

    def populate_arrays(self):
        self.arrays['x'].append(self.ctr)
        self.ctr += 0.1
        for j in range(1,len(self.axii_names)):
            self.arrays[self.axii_names[j]].append(randrange(10, 20))
        self.plot_key = True

    def plot_floats_for_PID(self):
        self.ax1.clear()
        self.ax2.clear()
        for i in range(1, self.num_of_axii - 2):
            self.ax1.plot(self.arrays[self.axii_names[0]], self.arrays[self.axii_names[i]],
                          label=self.labels[i - 1])
        self.ax1.legend(loc="upper right")
        self.ax2.plot(self.arrays['x'], self.arrays['e'], label=self.labels[2])
        self.ax2.plot(self.arrays['x'], self.arrays['c'], label=self.labels[3])
        self.ax2.legend(loc="upper right")

        self.ax1.set_ylabel("RPS")
        self.ax2.set_xlabel("Seconds")
        self.ax2.set_ylabel("Commands and Error")
        self.ax2.set_title('Error Accumulation', color='b')
        self.ax1.set_title('PID Performance', color='c')
        my_max = self.arrays[self.axii_names[0]][-1]
        self.ax1.set_xlim([my_max - 20, my_max])
        self.ax2.set_xlim([my_max - 20, my_max])

    def plot_uint16s(self):
        self.ax1.clear()
        for i in range(1, self.num_of_axii):
            self.ax1.plot(self.arrays[self.axii_names[0]], self.arrays[self.axii_names[i]],
                          label=self.labels[i - 1])
        self.ax1.legend(loc="upper right")

        self.ax1.set_ylabel("Duty Cycles")
        self.ax1.set_title('Index', color='c')
        #my_max = self.arrays[self.axii_names[0]][-1]
        #self.ax1.set_xlim([my_max - 20, my_max])
        self.fig.gca().relim()
        self.fig.gca().autoscale_view()

    def animate(self, i):
        if self.plot_key_counter == 0:
            for i in range(1489):
                self.get_println_data()
        #print(len(self.arrays['x']))
        else:
            self.get_println_data()

        # self.populate_arrays()
        if self.plot_key:
            self.plot_uint16s()
            self.plot_key = False
            self.plot_key_counter = 1

myObj = MyClass()
