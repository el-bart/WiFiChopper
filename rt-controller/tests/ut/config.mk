# name of the application
export PROGRAM_NAME:=test_program

# toolchain
export CC :=gcc
export CXX:=g++
export LD :=$(CXX)

# extra flags, if needed
export CFLAGS  +=
export CXXFLAGS+=-std=c++11
export LDFLAGS +=
