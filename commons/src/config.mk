# name of the application
export PROGRAM_NAME:=uChaos

# toolchain
export CC :=gcc
export CXX:=g++
export LD :=$(CXX)

# extra flags, if needed
export CFLAGS  +=
export CXXFLAGS+=-std=c++0x
export LDFLAGS +=
