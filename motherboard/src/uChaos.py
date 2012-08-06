#!/usr/bin/python

import Communication
import Logger
import socket
import serial
import sys


if len(sys.argv) != 1+4:
    print(sys.argv[0] + " <host> <port> <keyFile> <rt-board-dev>")
    sys.exit(1)


class Server:
    def __init__(self, host, port, key, rtbDev):
        # open serial device
        self.dev = serial.Serial(port=rtbDev, baudrate=38400, timeout=1)
        # open socket for incomming connections
        Logger.info("listening on " + host + ":" + str(port))
        self.key = key
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
            cmd = comm.recv()
            Logger.info("got request: " + cmd.replace("\n", ""))
            self.dev.write(cmd)
            resp = self.dev.readline()
            Logger.info("sending response: " + resp.replace("\n", ""))
            comm.send(resp)



srv = Server(sys.argv[1], int(sys.argv[2]), Communication.readKeyFromFile(sys.argv[3]), sys.argv[4])

while True:
    try:
        srv.processClient()
    except Exception as ex:
        Logger.error(str(ex))
