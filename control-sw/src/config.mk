# name of the application
export PROGRAM_NAME:=control-sw

# toolchain
export CC :=gcc
export CXX:=g++
export LD :=$(CXX)

# extra flags, if needed
export CFLAGS  +=
export CXXFLAGS+=-std=c++11
export LDFLAGS +=-lopencv_highgui -lopencv_core -lopencv_imgproc -lmcrypt -lrt
