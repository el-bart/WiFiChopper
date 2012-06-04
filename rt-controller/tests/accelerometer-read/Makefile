#MCU=atmega16
MCU=atmega8
CXX=avr-g++
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS= -mmcu=$(MCU) -Wall -Wextra -Os -mcall-prologues
#-------------------
all: accel.hex accel.bin
#-------------------
accel.hex : accel.out
	$(OBJCOPY) -R .eeprom -O ihex accel.out accel.hex
accel.out : accel.o
	$(CXX) $(CFLAGS) -o accel.out -Wl,-Map,accel.map accel.o
accel.bin : accel.out
	$(OBJCOPY) -O binary $< $@
accel.o : accel.cpp
	$(CXX) $(CFLAGS) -Os -c accel.cpp
# you need to erase first before loading the program.
# load (program) the software into the eeprom:
#load: accel.hex
#	uisp -dlpt=/dev/parport0 --erase  -dprog=dapa
#	uisp -dlpt=/dev/parport0 --upload if=accel.hex -dprog=dapa  -v=3 --hash=32

flash_mcu:
	avrdude -P avrdoper -c stk500v2 -p m8 -U flash:w:accel.bin
	#avrdude -P avrdoper -c stk500v2 -p m8 -U lfuse:w:255:m
	avrdude -P avrdoper -c stk500v2 -p m8 -U lfuse:w:0xCC:m -U hfuse:w:0xD9:m

#-------------------
clean:
	rm -fv *.o *.map *.out *.hex *.bin
#-------------------
