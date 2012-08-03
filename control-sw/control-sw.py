#!/usr/bin/python

import Communication
import Logger
import socket
import sys


protocolVersion = 1;

if len(sys.argv) != 1+3:
    print(sys.argv[0] + " <host> <port> <keyFile>")
    sys.exit(1)


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
        Logger.info("checking other endpoint")
        self.comm.send( (protocolVersion, "hello") )
        resp = self.__getResponse()
        if resp[1] != "uChaos awaits orders...":
            raise Exception("invalid device - unexpected response received")
        return (resp[2], resp[3], resp[4])

    def setSpeed(self, main1, main2, rear):
        Logger.info("setting speed: M1=" + str(main1) + ", M2=" + str(main2) + ", R=" + str(rear))
        if main1 < 0 or 255 < main1 or main2 < 0 or 255 < main2 or rear < -255 or 255 < rear:
            raise Exception("requested speed is out of range")
        self.comm.send( (protocolVersion, "set speed", main1, main2, rear) )
        resp = self.__getResponse()
        if resp[1] != "ok":
            raise Exception("invalid response: " + str(resp[1]))

    def getSpeed(self):
        Logger.info("getting speed")
        self.comm.send( (protocolVersion, "get speed") )
        resp = self.__getResponse()
        if len(resp) != 1+3:
            raise Exception("invalid response: " + str(resp))
        return (resp[1], resp[2], resp[3])

    def getVoltage(self):
        Logger.info("getting input voltage")
        self.comm.send( (protocolVersion, "Vin") )
        resp = self.__getResponse()
        if len(resp) != 1+1:
            raise Exception("invalid response: " + str(resp))
        return resp[1]

    def bye(self):
        Logger.info("disconnection from server")
        self.comm.send( (protocolVersion, "bye") )
        resp = self.__getResponse()
        if resp[1] != "bye, bye!":
            raise Exception("invalid response: " + str(resp))

    def __getResponse(self):
        resp = self.comm.recv()
        if resp[0] != protocolVersion:
            raise Exception("unknown version number: " + str(resp[0]))
        return resp


cln = Client(sys.argv[1], int(sys.argv[2]), Communication.readKeyFromFile(sys.argv[3]) )

cln.bye()
