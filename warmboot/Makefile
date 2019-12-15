TOOLCHAIN = arm-none-eabi-

CC = ${TOOLCHAIN}gcc
AS = ${TOOLCHAIN}as
LD = ${TOOLCHAIN}ld
OBJCOPY = ${TOOLCHAIN}objcopy

FLAGS := -march=armv5te

CFLAGS := ${FLAGS}
ASFLAGS := ${FLAGS}
LDFLAGS := ${FLAGS}

all: warmboot.bin

warmboot.o: warmboot.s
	${AS} ${ASFLAGS} warmboot.s -o warmboot.o

warmboot.elf: warmboot.o
	${LD} warmboot.o -o warmboot.elf # ld -Ttext 200000

warmboot.bin: warmboot.elf
	${OBJCOPY} -O binary warmboot.elf warmboot.bin
