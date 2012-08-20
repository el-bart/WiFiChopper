# name of the application
export PROGRAM_NAME:=control-sw

# toolchain
export CC :=gcc
export CXX:=g++
export LD :=$(CXX)

LIBAV_FLAGS:=$(shell pkg-config libavutil --cflags)
LIBAV_LIBS :=$(shell pkg-config libavutil --libs)

# extra flags, if needed
export CFLAGS  +=
export CXXFLAGS+=-std=c++11 $(LIBAV_FLAGS)
export LDFLAGS +=-lopencv_highgui -lopencv_core -lopencv_imgproc -lmcrypt $(LIBAV_LIBS) -lrt
