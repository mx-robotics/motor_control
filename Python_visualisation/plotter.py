import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from random import randrange
import serial
import serial.tools.list_ports
import struct


class MyClass:
    def __init__(self):
        self.fig = plt.figure()
        # plt.title('Interesting Graph\nCheck it out')
        self.ax1 = self.fig.add_subplot(2, 1, 1)
        self.ax2 = self.fig.add_subplot(2, 1, 2)
        # for ax in self.ax1:
        #   print(ax)
        self.axii_names = ['x', 'y', 'z', 'e', 'c']
        self.labels = ["Measured", "Target", "Error", "Command"]
        self.num_of_axii = len(self.axii_names)
        self.arrays = {}
        self.create_the_arrays()
        self.ctr = 0
        self.plot_key = False
        self.bytes_to_receive = 160

        self.portName = ""

        # self. baudrate = 115200

        self.ser = None
        self.establish_serial_port_com("ttyACM0")
        self.ani_func = FuncAnimation(self.fig, self.animate, interval=100)

        plt.legend()
        plt.show()

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
            if "/dev/ttyACM0" in port.device:
                self.portName = port.device

    def create_the_arrays(self):
        for i in range(self.num_of_axii):
            axis_name = self.axii_names[i]
            self.arrays[axis_name] = []

    def get_serial_data_to_populate_arrays(self):
        if self.ser.in_waiting:  # Check for data not for an open port
            num_bytes_received = self.ser.in_waiting
            b1 = self.ser.read(self.ser.in_waiting)
            i = 0
            while i < (num_bytes_received / 4):
                try:
                    [k] = struct.unpack('f', b1[i * 4:(i + 1) * 4])
                    self.arrays['y'].append(k)
                    i += 1
                    [l] = struct.unpack('f', b1[i * 4:(i + 1) * 4])
                    self.arrays['z'].append(l)
                    i += 1

                    [m] = struct.unpack('f', b1[i * 4:(i + 1) * 4])
                    self.arrays['e'].append(m)
                    i += 1

                    [n] = struct.unpack('f', b1[i * 4:(i + 1) * 4])
                    self.arrays['c'].append(n)
                    i += 1

                    self.arrays['x'].append(self.ctr)
                    self.ctr += 0.1


                except struct.error:
                    print("i = ", i)
                    print("exception thrown")
                    pass
            self.plot_key = True

    def populate_arrays(self):
        self.arrays['x'].append(self.ctr)
        self.ctr += 0.1
        self.arrays['y'].append(randrange(10, 20))
        self.arrays['z'].append(randrange(10, 20))
        self.arrays['e'].append(randrange(10, 20))
        self.arrays['c'].append(randrange(10, 20))
        self.plot_key = True;

    def animate(self, i):
        self.get_serial_data_to_populate_arrays()
        # self.populate_arrays()
        if self.plot_key:
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

            # self.fig.gca().relim()
            # self.fig.gca().autoscale_view()
            self.plot_key = False


myObj = MyClass()
