CROSS_COMPILE=arm-none-eabi-
CFLAGS=-O2 -g -DIN_PSP -I../include -I../Lib/include -std=gnu99 -fomit-frame-pointer -nostartfiles -ffreestanding -Wextra -Werror
VPATH=../Lib/src

OBJS = main.o string.o svc.o

all : ecc-test.elf ecc-test.raw

clean:
	rm -f _start.o $(OBJS)

%.o: %.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CROSS_COMPILE)gcc $(CFLAGS) -c -o $@ $^

_start.o: ../Lib/_start.S
	$(CROSS_COMPILE)as -march=armv7-a -mcpu=cortex-a8 -o $@ $^

main.o: main.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -c -o $@ main.c

ecc-test.elf : ../build/linker.ld _start.o $(OBJS)
	$(CROSS_COMPILE)ld -T $^ -o $@

ecc-test.raw: ecc-test.elf
	$(CROSS_COMPILE)objcopy -O binary $^ tmp
	dd if=/dev/zero of=$@ bs=1 count=256
	cat tmp >>$@
	rm tmp


