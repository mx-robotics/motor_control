
#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that listens to std_msgs/Strings published 
## to the 'chatter' topic

import rospy
import serial.tools.list_ports
from std_msgs.msg import String
from sensor_msgs.msg import Joy


def callback(data, arg1):
    # rospy.loginfo(" ".join(str(x) for x in data.axes))
    # rospy.loginfo(" ".join(str(x) for x in data.buttons))
    if (data.buttons[4]):  # dead mans button "LB"
        rospy.loginfo("Button pressed")
        x_cords = -1 * int(data.axes[3] * 100)  # gotta reverse
        y_cords = int(data.axes[4] * 100)

        x_cords_str = ""
        y_cords_str = ""

        # rospy.loginfo("X-Coordinate : %s" %x_cords)
        # rospy.loginfo("Y-Coordinate : %s" %y_cords)
        if x_cords == 100:
            x_cords_str = "+100"
        elif x_cords == -100:
            x_cords_str = "-100"
        elif x_cords < 100 and x_cords > 10:
            x_cords_str = "+0" + str(x_cords)
        elif x_cords < 10 and x_cords >= 0:
            x_cords_str = "+00" + str(x_cords)
        elif x_cords > -100 and x_cords < -10:
            x_cords_str = "-0" + str(-1 * x_cords)
        elif x_cords > -10 and x_cords < 0:
            x_cords_str = "-00" + str(-1 * x_cords)

        if y_cords == 100:
            y_cords_str = "+100"
        elif y_cords == -100:
            y_cords_str = "-100"
        elif y_cords < 100 and y_cords > 10:
            y_cords_str = "+0" + str(y_cords)
        elif y_cords < 10 and y_cords >= 0:
            y_cords_str = "+00" + str(y_cords)
        elif y_cords > -100 and y_cords < -10:
            y_cords_str = "-0" + str(-1 * y_cords)
        elif y_cords > -10 and y_cords < 0:
            y_cords_str = "-00" + str(-1 * y_cords)

        rospy.loginfo("X-Coordinate STR :%s" % x_cords_str)
        rospy.loginfo("Y-Coordinate STR: %s" % y_cords_str)

        serial_str = x_cords_str + y_cords_str
        rospy.loginfo("str to send : %s" % serial_str)
        ser = arg1
        ser.write(serial_str)


def listener(arg1):
    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('joy_to_serial', anonymous=True)

    rospy.Subscriber('joy', Joy, callback, arg1)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


def open_ser_port():
    com_port_name = ""
    for comport in serial.tools.list_ports.comports():
        if ("/dev/ttyACM" in comport.device):
            com_port_name = comport.device
            print com_port_name
    ser = serial.Serial(
        port=com_port_name,
        baudrate=9600,
        parity=serial.PARITY_ODD,
        stopbits=serial.STOPBITS_TWO,
        bytesize=serial.SEVENBITS
    )
    return ser


if __name__ == '__main__':
    listener(open_ser_port())
