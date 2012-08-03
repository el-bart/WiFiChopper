#!/usr/bin/python

from Crypto        import Random
from Crypto.Cipher import AES


class CryptoMod:
    def __init__(self, key):
        self.iv     = Random.new().read(AES.block_size)
        self.cipher = AES.new(key, AES.MODE_CFB, self.iv)

    def encrypt(self, str):
        return self.cipher.encrypt(self.iv + str)

    def decrypt(self, str):
        return self.cipher.decrypt(str)[AES.block_size:]


def readKeyFromFile(path):
    f = open(path, 'rb')
    return f.read()


class Communicator:
    def __init__(self, sock, key):
        self.sock  = sock
        self.crypt = CryptoMod(key)

    def send(self, cmd):
        enc = self.crypt.encrypt(cmd)
        self.sock.sendall(enc)

    def recv(self):
        while True:
            enc = self.sock.recv(1024)
            if len(enc) > 0:
                break
        resp = cjson.decode(enc)
        return resp
