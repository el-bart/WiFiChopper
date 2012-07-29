# name of the application
export PROGRAM_NAME:=rt-controller

# toolchain
export CC :=avr-gcc
export CXX:=avr-g++
export OBJCOPY:=avr-objcopy
export LD :=$(CC)

# tools settings
export MCU_TYPE:=atmega8

# extra flags, if needed
export CFLAGS  +=-mmcu=$(MCU_TYPE)
export CXXFLAGS+=-mmcu=$(MCU_TYPE) -std=c++11
export LDFLAGS +=-mmcu=$(MCU_TYPE)
