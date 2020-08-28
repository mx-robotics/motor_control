# !/usr/bin/env python
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


def translate(value, leftMin, leftMax, rightMin, rightMax):
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)


def translate_joystick_y_to_rpm(joyvalue_Y, joy_min=-100, joy_max=100, rps_min=-6.5, rps_max=6.5):
    return translate(joyvalue_Y, joy_min, joy_max, rps_min, rps_max)


def translate_joystick_x_to_servo_angle(joy_value_x, joy_min=-100, joy_max=100, angle_min=-30, angle_max=30):
    return translate(joy_value_x, joy_min, joy_max, angle_min, angle_max)


class ServoAngleParser:
    def __init__(self):
        self.minServoAngleOnRaceCar = -30
        self.minServoAngleOnTeensyServoLib = 100
        self.maxServoAngleOnRacecar = 30
        self.minServoAngleOnTeensyServoLib = 160
        self.difference = self.minServoAngleOnTeensyServoLib - self.maxServoAngleOnRacecar

    def convertRealServoAngleToTeebsyLibCommand(self, angle):
        angle = angle * -1
        if angle > 30 or angle > 30:
            raise ValueError("Angle should be between -30 and +30")
        else:
            return angle + self.difference


class SerialMessageParserForTeensy:
    def __init__(self):
        self.ServoCommandParser = ServoAngleParser()

    def padWithZeros(self, msg):
        if len(msg) > 3:
            return msg[:3]
        elif len(msg) == 3:
            return msg
        elif len(msg) == 2:
            return '0' + msg
        elif len(msg) == 1:
            return '00' + msg

    def parseAngle(self, angle):
        tmp = str(int(self.ServoCommandParser.convertRealServoAngleToTeebsyLibCommand(angle)))
        return self.padWithZeros(tmp)

    def parseRPS(self, rps):
        if rps < 0:
            rps *= -1
        temp_rps = str(int(rps * 100))
        return self.padWithZeros(temp_rps)

    def parseDirection(self, rps):
        if rps > 0:
            return 'F'
        elif rps < 0:
            return 'B'
        else:
            return 'S'

    def constructCompleteCmd(self, rps, angle):
        msg_str = self.parseDirection(rps) + + self.parseRPS(rps) + self.parseAngle(angle)
        return msg_str


def callback(data, serial, msg_parser):
    # rospy.loginfo(" ".join(str(x) for x in data.axes))
    # rospy.loginfo(" ".join(str(x) for x in data.buttons))
    if data.buttons[4]:  # dead mans button "LB"
        rospy.loginfo("Button pressed")
        x_cords = -1 * int(data.axes[3] * 100)  # gotta reverse
        y_cords = int(data.axes[4] * 100)
        rps = translate_joystick_y_to_rpm(y_cords)
        servo_ange = translate_joystick_x_to_servo_angle(x_cords)
        teensy_cmd = msg_parser.constructCompleteCmd(rps, servo_ange)

        # rospy.loginfo("X-Coordinate STR :%s" % x_cords_str)
        # rospy.loginfo("Y-Coordinate STR: %s" % y_cords_str)

        rospy.loginfo("str to send : %s" % teensy_cmd)
        serial.write(teensy_cmd.encode())


def listener(serial, msg_parser):

    rospy.init_node('joy_to_serial', anonymous=True)

    rospy.Subscriber('joy', Joy, callback, serial, msg_parser)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


def open_ser_port():
    com_port_name = ""
    for comport in serial.tools.list_ports.comports():
        if "/dev/ttyACM" in comport.device:
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
    teensy_msg_parser = SerialMessageParserForTeensy()
    serial_port = open_ser_port()
    listener(serial_port, teensy_msg_parser)
