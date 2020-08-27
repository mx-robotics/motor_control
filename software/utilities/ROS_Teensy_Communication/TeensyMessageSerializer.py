import serial.tools.list_ports
import time

teensyComport = '/dev/ttyACM0'
for i in serial.tools.list_ports.comports():
    print(i.device)
    if 'ttyACM' in i.device:
        teensyComport = i.device

ser = serial.Serial(teensyComport, 12000000, timeout=1)
print("serial connected")

N = 512


class ServoAngleParser():
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


class SerialMessageParserForTeensy():
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
        temp_rps = str(int(rps * 100))
        return self.padWithZeros(temp_rps)

    def constructCompleteCmd(self, direction, rps, angle):
        msg_str = direction + self.parseRPS(rps) + self.parseAngle(angle)
        return msg_str


mycls = SerialMessageParserForTeensy()
teensMsg = mycls.constructCompleteCmd('F', 5.22232, 30)

while True:
    #al = input("Enter your value: ")
    ser.write(teensMsg.encode())
    time.sleep(0.001)
    rec = ''
    while ser.in_waiting:
        rec += ser.read().decode()
    print(rec)

ser.close()
