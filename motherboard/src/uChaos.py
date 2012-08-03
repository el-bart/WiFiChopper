#!/usr/bin/python

import Communication
import Logger
import socket
import sys


protocolVersion=1
softwareVersion=[0,0,0]

if len(sys.argv) != 1+3:
    print(sys.argv[0] + " <host> <port> <keyFile>")
    sys.exit(1)


def helloHandle(comm, cmd):
    comm.send( (protocolVersion, "uChaos awaits orders...", softwareVersion[0], softwareVersion[1], softwareVersion[2]) )
    return True

def setSpeedHandle(comm, cmd):
    # TODO: set speed
    comm.send( (protocolVersion, "ok") )
    return True

def getSpeedHandle(comm, cmd):
    # TODO: get speed
    comm.send( (protocolVersion, 42, 43, -44) )
    return True

def getVoltageHandle(comm, cmd):
    # TODO: get voltage
    comm.send( (protocolVersion, 3.14) )
    return True

def byeHandle(comm, cmd):
    comm.send( (protocolVersion, "bye, bye!") )
    return False


class Server:
    def __init__(self, host, port, key):
        Logger.info("listening on " + host + ":" + str(port))
        self.key = key
        # prepare commands processing dispatching dictionary
        self.disp = {
                        "hello":helloHandle,
                        "set speed":setSpeedHandle,
                        "get speed":getSpeedHandle,
                        "Vin":getVoltageHandle,
                        "bye":byeHandle
                    }
        # open TCP port for connections
        self.lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.lsock.bind((host, port))
        self.lsock.listen(1)

    def processClient(self):
        Logger.info("waiting for incomming connections")
        sock,addr = self.lsock.accept()
        Logger.info("got new connection from " + str(addr))
        sock.settimeout(18.0)
        # use communication protocol
        comm = Communication.Communicator(sock, self.key)
        # wait for orders
        while True:
            Logger.info("awaiting new request")
            cmd = self.__recv(comm)
            Logger.info("got request: " + cmd[1])
            # TODO: response with an error, if no such key!
            if not self.disp[cmd[1]](comm, cmd):
                Logger.info("disconnectiong on user request")
                break

    def __recv(self, comm):
        cmd = comm.recv()
        if cmd[0] != protocolVersion:
            comm.send( (protocolVersion, "ERROR", "unsupported protocol version") )
            raise Exception("unsupported protocol version: " + str(cmd[0]))
        return cmd




srv = Server(sys.argv[1], int(sys.argv[2]), Communication.readKeyFromFile(sys.argv[3]) )

while True:
    try:
        srv.processClient()
    except Exception as ex:
        Logger.error(str(ex))
