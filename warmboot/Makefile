TOOLCHAIN = arm-none-eabi-

CC = ${TOOLCHAIN}gcc
AS = ${TOOLCHAIN}as
LD = ${TOOLCHAIN}ld
OBJCOPY = ${TOOLCHAIN}objcopy

FLAGS := -march=armv5te

CFLAGS := ${FLAGS} -O3 -ffreestanding -fno-builtin
ASFLAGS := ${FLAGS}
LDFLAGS := ${FLAGS}

all: warmboot.bin

warmboot.o: warmboot.s
	${AS} ${ASFLAGS} warmboot.s -o warmboot.o

main.o: main.c
	${CC} ${CFLAGS} -c main.c -o main.o

warmboot.elf: warmboot.o main.o
	${LD} warmboot.o main.o -o warmboot.elf -Ttext 86000000

warmboot.bin: warmboot.elf
	${OBJCOPY} -O binary warmboot.elf warmboot.bin

install: warmboot.bin
	# hacky helper customised to my local machine, to copy to SD
	sudo mount /dev/mmcblk0 /mnt/tmp/ && sudo cp warmboot.bin /mnt/tmp && sudo umount /mnt/tmp

clean:
	rm -f warmboot.bin warmboot.o main.o warmboot.elf
