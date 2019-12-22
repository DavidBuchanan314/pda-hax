TOOLCHAIN = arm-none-eabi-

CC = ${TOOLCHAIN}gcc
AS = ${TOOLCHAIN}as
LD = ${TOOLCHAIN}ld
OBJCOPY = ${TOOLCHAIN}objcopy

FLAGS := -march=armv5te

CFLAGS := ${FLAGS} -O3 -ffreestanding -fno-builtin
ASFLAGS := ${FLAGS}
LDFLAGS := ${FLAGS}

OBJS := pe.o main.o zImage.o

all: kexec.exe

%.o: %.s
	${AS} ${ASFLAGS} $< -o $@

%.o: %.c
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
