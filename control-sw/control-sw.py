#!/usr/bin/python

import Communication
import Logger
import socket
import sys


if len(sys.argv) != 1+3:
    print(sys.argv[0] + " <host> <port> <keyFile>")
    sys.exit(1)


def __toHex(n):
    return "{0:+#05x}".format(n)

def __toSHex(n):
    return "{0:#05x}".format(n)

def __fromHex(s):
    int(s, 16)


class Client:
    def __init__(self, host, port, key):
        Logger.info("connecting to " + host + ":" + str(port))
        # preapre TCP connection
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((host, port))
        sock.settimeout(1.5)
        # use communication protocol
        self.comm = Communication.Communicator(sock, key)
        # check if endpoint is the right one
        self.hello()

    def hello(self):
        self.comm.send("hello\n")
        resp = self.comm.recv()
        if resp.split()[0] != "WiFiChopper":
            raise Exception("invalid response from device: " + resp)
        # return array of version numbers
        v = resp.split()[2][1:].split('.')
        # parse as numbers
        for i in range(0:3):
          v[i] = __fromHex(v[i])
        return v;

    def accel(self):
        self.comm.send("accel?\n")
        resp = self.comm.recv()
        v = respo.split()
        if len(v) != 3:
            raise Exception("invalid response: " + resp)
        # parse as numbers
        for i in range(0:3):
          v[i] = __fromHex(v[i])
        return v;

    def getSpeed(self):
        self.comm.send("eng?\n")
        resp = self.comm.recv()
        v = respo.split()
        if len(v) != 3:
            raise Exception("invalid response: " + resp)
        # parse as numbers
        for i in range(0:3):
          v[i] = __fromHex(v[i])
        return v;

    def setSpeed(self, main1, main2, rear):
        self.comm.send("engset " + toHex(main1) + " " + toHex(main2) + toSHex(rear) + "\n")
        resp = self.comm.recv()
        if resp != "set" + __toHex(main1) + __toHex(main2) + toSHex(rear):
            raise Exception("invalid response: " + resp)

    def getVoltage(self):
        self.comm.send("vin?\n")
        resp = self.comm.recv()
        return __fromHex(resp)


cln = Client(sys.argv[1], int(sys.argv[2]), Communication.readKeyFromFile(sys.argv[3]) )

cln.hello()
cln.setSpeed(1,2,3)
print( cln.getSpeed() )
print( cln.getVoltage() )
print( cln.getAccel() )
cln.bye()
