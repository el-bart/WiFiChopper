#!/usr/bin/python
import time
import serial


def __toHex(n):
    return "{0:+#05x}".format(n)

def __toSHex(n):
    return "{0:#05x}".format(n)

def __fromHex(s):
    int(s, 16)


class RTBoardComm:
    def __init__(self, devPath):
        self.dev = serial.Serial(port=devPath, baurdate=38400, timeout=1)
        self.hello()

    def hello(self):
        self.dev.write("hello\n")
        resp = self.dev.readline()
        if resp.split()[0] != "WiFiChopper":
            raise Exception("invalid response from device: " + resp)
        # return array of version numbers
        v = resp.split()[2][1:].split('.')
        # parse as numbers
        for i in range(0:3):
          v[i] = __fromHex(v[i])
        return v;

    def accel(self):
        self.dev.write("accel?\n")
        resp = self.dev.readline()
        v = respo.split()
        if len(v) != 3:
            raise Exception("invalid response: " + resp)
        # parse as numbers
        for i in range(0:3):
          v[i] = __fromHex(v[i])
        return v;

    def getSpeed(self):
        self.dev.write("eng?\n")
        resp = self.dev.readline()
        v = respo.split()
        if len(v) != 3:
            raise Exception("invalid response: " + resp)
        # parse as numbers
        for i in range(0:3):
          v[i] = __fromHex(v[i])
        return v;

    def setSpeed(self, main1, main2, rear):
        self.dev.write("engset " + toHex(main1) + " " + toHex(main2) + toSHex(rear)
        # TODO
        pass

    def getVoltage(self):
        # TODO
        pass
