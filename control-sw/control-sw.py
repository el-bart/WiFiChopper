#!/usr/bin/python

import Communication
import Logger
import socket
import sys


if len(sys.argv) != 1+3:
    print(sys.argv[0] + " <host> <port> <keyFile>")
    sys.exit(1)


def toHex(n):
    return "{0:#04x}".format(n)[2:]

def toSHex(n):
    sig = '-' if n<0 else '+'
    return sig + toHex(n)

def fromHex(s):
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
            raise Exception("invalid 'hello' response: " + resp)
        # return array of version numbers
        v = resp.split()[2][1:].split('.')
        # parse as numbers
        for i in range(0,3):
          v[i] = fromHex(v[i])
        return v;

    def getAccel(self):
        self.comm.send("accel?\n")
        resp = self.comm.recv()
        v = resp.split()
        if len(v) != 3:
            raise Exception("invalid accelerometer read response: " + resp)
        # parse as numbers
        for i in range(0,3):
          v[i] = fromHex(v[i])
        return v;

    def getSpeed(self):
        self.comm.send("eng?\n")
        resp = self.comm.recv()
        v = resp.split()
        if len(v) != 3:
            raise Exception("invalid engine speed query response: " + resp)
        # parse as numbers
        for i in range(0,3):
          v[i] = fromHex(v[i])
        return v;

    def setSpeed(self, main1, main2, rear):
        self.comm.send("engset " + toHex(main1) + " " + toHex(main2) + " " + toSHex(rear) + "\n")
        resp = self.comm.recv()
        if resp != "set" + toHex(main1) + toHex(main2) + toSHex(rear) + "\n":
            raise Exception("invalid engine speed set response: " + resp)

    def getVoltage(self):
        self.comm.send("vin?\n")
        resp = self.comm.recv()
        return fromHex(resp)


cln = Client(sys.argv[1], int(sys.argv[2]), Communication.readKeyFromFile(sys.argv[3]) )

cln.hello()
cln.setSpeed(1,2,3)
print( cln.getSpeed() )
print( cln.getVoltage() )
print( cln.getAccel() )
