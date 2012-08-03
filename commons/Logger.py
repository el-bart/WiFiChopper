#!/usr/bin/python

def info(msg):
    __logImpl("info", msg)

def warn(msg):
    __logImpl("WARNING", msg)

def error(msg):
    __logImpl("ERROR", msg)


# TODO
def __logImpl(pri, msg):
    print(pri + ": " + msg)
