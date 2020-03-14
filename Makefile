C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

CC = i686-elf-gcc
GDB = i686-elf-gdb

# -g: Use debugging symbols in gcc
CFLAGS = -g

# First rule is run by default
os-image.bin: boot/boot_sector.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

run: os-image.bin
	qemu-system-i386 -fda os-image.bin
	#sudo qemu-system-i386 -net nic -netdev tap,id=br0 -nic model=rtl8139 -fda os-image.bin

## Used for debugging purposes
#kernel.elf: boot/kernel_entry.o ${OBJ}
#	i686-elf-ld -o $@ -Ttext 0x1000 $^ 
#
## Open the connection to qemu and load our kernel-object file with symbols
#debug: os-image.bin kernel.elf
#	qemu-system-i686 -s -fda os-image.bin -d guest_errors,int &
#	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
