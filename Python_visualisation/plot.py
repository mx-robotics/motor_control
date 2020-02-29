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
        self.ax1 = self.fig.add_subplot(1, 1, 1)
        self.ax2 = None
        # for ax in self.ax1:
        #   print(ax)
        self.arrays = {}
        self.ctr = 0
        self.plot_key = False
        self.portName = ""
        self.x_axis = []
        self.y_axis = []
        self.ser = None
        self.ctr  = 0
        self.establish_serial_port_com("ttyACM0")
        self.get_data()

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
            if kw in port.device:
                self.portName = port.device

    def get_data(self):
        while True:
            if self.ser.in_waiting:  # Check for data not for an open port
                num_bytes_received = self.ser.in_waiting
                b1 = self.ser.read(self.ser.in_waiting)
                try:
                    [k] = struct.unpack('f', b1[0:4])
                    self.x_axis.append(k)
                    [k] = struct.unpack('f', b1[4:8])
                    self.y_axis.append(k)

                except:
                    print("error")

            if len(self.x_axis) == 100:
                self.ax1.clear()
                print(self.x_axis)
                print(self.y_axis)
                self.ax1.plot(self.x_axis[2:], self.y_axis[2:], label = "RPS vs Speed Command ")
                self.ax1.set_ylabel("RPS")
                self.ax1.set_xlabel("Speed Command")
                break




myObj = MyClass()
