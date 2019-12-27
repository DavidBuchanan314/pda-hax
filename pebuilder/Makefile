TOOLCHAIN = arm-none-eabi-

CC = ${TOOLCHAIN}gcc
AS = ${TOOLCHAIN}as
LD = ${TOOLCHAIN}ld
OBJCOPY = ${TOOLCHAIN}objcopy

FLAGS := -march=armv5te

CFLAGS := ${FLAGS} -fPIC -fPIE -I./include -std=gnu99 -O3 -ffreestanding -Wall -Wextra
ASFLAGS := ${FLAGS}
LDFLAGS := ${FLAGS}

OBJS := pe.o main.o uart.o mmu.o string.o zImage.o

all: kexec.exe

zimage.o: zImage

%.o: %.s
	${AS} ${ASFLAGS} $< -o $@

%.o: %.c $(wildcard include/*.h)
	${CC} ${CFLAGS} -c $< -o $@

kexec.elf: ${OBJS} script.lds
	${LD} ${OBJS} -o kexec.elf -T script.lds

kexec.exe: kexec.elf
	${OBJCOPY} -O binary kexec.elf kexec.exe

install: kexec.exe
	# hacky helper customised to my local machine, to copy to SD
	mount /dev/mmcblk0 /mnt/tmp/
	cp kexec.exe /mnt/tmp
	umount /mnt/tmp

clean:
	rm -f kexec.exe kexec.elf ${OBJS}
