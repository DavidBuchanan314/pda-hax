TOOLCHAIN = arm-none-eabi-

CC = ${TOOLCHAIN}gcc
AS = ${TOOLCHAIN}as
LD = ${TOOLCHAIN}ld
OBJCOPY = ${TOOLCHAIN}objcopy

FLAGS := -march=armv5te

CFLAGS := ${FLAGS} -O3 -ffreestanding -fno-builtin
ASFLAGS := ${FLAGS}
LDFLAGS := ${FLAGS}

all: kexec.exe

pe.o: pe.s
	${AS} ${ASFLAGS} pe.s -o pe.o

zImage.o: zImage.s
	${AS} ${ASFLAGS} zImage.s -o zImage.o

main.o: main.c
	${CC} ${CFLAGS} -c main.c -o main.o

kexec.elf: pe.o main.o zImage.o
	${LD} pe.o main.o zImage.o -o kexec.elf -T script.lds

kexec.exe: kexec.elf
	${OBJCOPY} -O binary kexec.elf kexec.exe

install: kexec.exe
	# hacky helper customised to my local machine, to copy to SD
	mount /dev/mmcblk0 /mnt/tmp/
	cp kexec.exe /mnt/tmp
	umount /mnt/tmp

clean:
	rm -f kexec.exe pe.o main.o kexec.elf
