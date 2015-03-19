# name of the application
export PROGRAM_NAME:=control-sw

# default toolchain - can be changed with 'make TC=xxx'
export TC:=gcc

# toolchain
ifeq ($(TC),gcc)
export CC :=gcc
export CXX:=g++
export LD :=$(CXX)
else
ifeq ($(TC),avr)
# tools
export CC :=avr-gcc
export CXX:=avr-g++
export OBJCOPY:=avr-objcopy
export LD :=$(CC)
# tools settings
export LFUSE:=255
export HFUSE:=255
# type of MCU for the avrdude programmer
export MCU_PROG_TYPE:=m8
# mype of MCU for the compiler
export MCU_TYPE:=atmega8
export CFLAGS  +=-mmcu=$(MCU_TYPE)
export CXXFLAGS+=-mmcu=$(MCU_TYPE)
export LDFLAGS +=-mmcu=$(MCU_TYPE)
else
$(error UNKNWON TOOLCHAIN '$(TC)'; USE 'gcc' (i.e. local) or 'avr')
endif # TC=avr
endif # TC=gcc

# av lib
LIBAV_FLAGS:=$(shell pkg-config libavutil --cflags)
LIBAV_LIBS :=$(shell pkg-config libavutil --libs)

# extra flags, if needed
export CFLAGS  +=
export CXXFLAGS+=-std=c++11 $(LIBAV_FLAGS)
export LDFLAGS +=-lopencv_highgui -lopencv_core -lopencv_imgproc -lv4l2 -lmcrypt $(LIBAV_LIBS) -lpthread -lrt

# default target
export DEFAULT_TARGET:=release
