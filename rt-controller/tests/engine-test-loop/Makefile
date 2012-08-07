#MCU=atmega16
MCU=atmega8
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS= -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
#-------------------
all: engines.hex engines.bin
#-------------------
engines.hex : engines.out
	$(OBJCOPY) -R .eeprom -O ihex engines.out engines.hex
engines.out : engines.o
	$(CC) $(CFLAGS) -o engines.out -Wl,-Map,engines.map engines.o
engines.bin : engines.out
	$(OBJCOPY) -O binary $< $@
engines.o : engines.c
	$(CC) $(CFLAGS) -Os -c engines.c
# you need to erase first before loading the program.
# load (program) the software into the eeprom:
#load: engines.hex
#	uisp -dlpt=/dev/parport0 --erase  -dprog=dapa
#	uisp -dlpt=/dev/parport0 --upload if=engines.hex -dprog=dapa  -v=3 --hash=32

flash_mcu:
	avrdude -P avrdoper -c stk500v2 -p m8 -U flash:w:engines.bin
	#avrdude -P avrdoper -c stk500v2 -p m8 -U lfuse:w:255:m
	avrdude -P avrdoper -c stk500v2 -p m8 -U lfuse:w:0xCC:m -U hfuse:w:0xD9:m

#-------------------
clean:
	rm -fv *.o *.map *.out *.hex *.bin
#-------------------
