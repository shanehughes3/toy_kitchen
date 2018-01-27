PRJNAME 	= toy_kitchen
MCU_TARGET 	= attiny2313
PROGRAMMER	= usbasp

OPTIMIZE	= -Os
CC		= avr-gcc
OBJCOPY		= avr-objcopy
OBJDUMP		= avr-objdump
CFLAGS		= -g -mmcu=${MCU_TARGET} -Wall ${OPTIMIZE} -Wno-main

${PRJNAME}.hex : ${PRJNAME}.elf
	${OBJCOPY} -O ihex -R .eeprom ${PRJNAME}.elf ${PRJNAME}.hex
${PRJNAME}.elf : ${PRJNAME}.o
	${CC} ${CFLAGS} -o ${PRJNAME}.elf ${PRJNAME}.o
${PRJNAME}.o : main.c
	${CC} ${CFLAGS} -c main.c -o ${PRJNAME}.o

install :
	avrdude -c ${PROGRAMMER} -p ${MCU_TARGET} -U flash:w:${PRJNAME}.hex

clean :
	rm ${PRJNAME}.hex ${PRJNAME}.elf ${PRJNAME}.o
